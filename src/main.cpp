#include <Arduino.h>
#include <M5StickC.h>
#include <MadgwickAHRS.h>
#include "sensor/sensor.h"
#include "sensor/mag.h"
#include "sensor/accel.h"
#include "sensor/gyro.h"
#include "ble.h"

Sensor *mag = new MagSensor();
Sensor *acc = new AccelSensor();
Sensor *gyro = new GyroSensor();
BLE_MIDI *midi = new BLE_MIDI();

float ax, ay, az, gx, gy, gz, mx, my, mz;
float roll, pitch, yaw;

unsigned long t = millis();
int frame = 0;


Madgwick filter;

void setup()
{
  Serial.begin(115200);
  M5.begin();
  M5.Lcd.setRotation(3);
  if (!mag->init())
  {
    M5.Lcd.println("bmm150 error");
  }
  if (!acc->init() || !gyro->init())
  {
    M5.Lcd.println("M5StickC Imu Error");
  }

  midi->init();
  midi->connect();
  M5.Lcd.println("start");

  while (!acc->calibrate()){}
  while (!gyro->calibrate()){}

  M5.Lcd.println("start2");

  while (!mag->calibrate()){}

  M5.Lcd.println("done");
  filter.begin(100);
}

void loop()
{
  M5.update();
  M5.Lcd.setCursor(0, 0);
  acc->read(&ax, &ay, &az);
  gyro->read(&gx, &gy, &gz);
  mag->read(&mx, &my, &mz);

  filter.update(gx, gy, gz, ax, ay, az, mx, my, mz);

  roll = filter.getRoll();
  pitch = filter.getPitch();
  yaw = filter.getYaw();
  M5.Lcd.printf("%6.2f %6.2f %6.2f \n", roll, pitch, yaw);
  if(midi->isConnected()) {
    if(frame == 2) {
      midi->control(1, 10, (asin(roll / 180.0 * PI) + 1) / 2 * 120);
      frame = 0;
    } else frame++;
  }
  
  if(millis() - t < 10)
    delay(10 - (millis() - t));
  t = millis();
}