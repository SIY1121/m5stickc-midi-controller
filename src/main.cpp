#include <Arduino.h>
#include <M5StickC.h>
#include "sensor/sensor.h"
#include "sensor/mag.h"

Sensor *mag = new MagSensor();

float x, y, z;

void setup() {
  M5.begin();
  M5.Lcd.setRotation(3);
  if(!mag->init()) {
    M5.Lcd.println("bmm150 error");
  }
  M5.Lcd.println("start");
  while(!mag->calibrate()){}
  M5.Lcd.println("done");
}

void loop() {
  M5.Lcd.setCursor(0,20);
  mag->read(&x, &y, &z);
  M5.Lcd.printf("%6.2f %6.2f %6.2f \n", x, y, z);
  delay(5);
}