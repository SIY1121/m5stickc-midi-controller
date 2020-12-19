#include <Arduino.h>
#include <M5StickC.h>
#include "sensor/sensor.h"
#include "sensor/mag.h"
#include "sensor/accel.h"
#include "sensor/gyro.h"
#include "ble.h"
#include "status/CalibrationReady.h"

MagSensor mag = MagSensor();
AccelSensor acc = AccelSensor();
GyroSensor gyro = GyroSensor();
BLE_MIDI midi = BLE_MIDI();

Status *status = calibrationReadyStatus;

void setup()
{
  Serial.begin(115200);
  M5.begin();
  if (!mag.init())
  {
    M5.Lcd.println("bmm150 error");
  }
  if (!acc.init() || !gyro.init())
  {
    M5.Lcd.println("M5StickC Imu Error");
  }

  midi.init();
  midi.connect();
  M5.Lcd.fillScreen(TFT_DARKGREY);
  M5.Lcd.textbgcolor = TFT_DARKGREY;
}

bool isFirstPaint = true;

void loop()
{
  M5.update();
  Status *next = status->exec(acc,gyro,mag,midi, isFirstPaint);
  if(next != status) {
    status = next;
    M5.Lcd.fillScreen(TFT_DARKGREY);
    isFirstPaint = true;
  } else {
    isFirstPaint = false;
  }
}