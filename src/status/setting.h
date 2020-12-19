#pragma once

#include "status.h"
#include <Arduino.h>
#include <M5StickC.h>
#include <MadgwickAHRS.h>
#include "sensor/sensor.h"

struct SettingValue {
  uint8_t cc[4] = {1,2,3,4};
};

class SettingStatus: public Status {
  float ax, ay, az, gx, gy, gz, mx, my, mz;
  float roll, pitch, yaw;
  uint8_t selectedIndex = 0;
  bool edit = false;
  String menuItem[4] = {"Roll", "Pitch", "Yaw", "Btn"};
  unsigned long t;
  unsigned long lastTime;
  Sensor& acc;
  Sensor& gyro;
  Sensor& mag;
  Madgwick& filter;
  SettingValue& setting;
public:
  SettingStatus(Sensor &acc, Sensor &gyro, Sensor &mag, Madgwick& filter, SettingValue& setting);
  StatusNo exec(bool isFirstPaint);
};