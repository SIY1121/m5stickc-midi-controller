#pragma once

#include "sensor/sensor.h"
#include "status.h"
#include "setting.h"
#include "M5StickC.h"
#include <MadgwickAHRS.h>

class RunningStatus: public Status {
  float ax, ay, az, gx, gy, gz, mx, my, mz;
  float roll, pitch, yaw;
  uint8_t cc1,cc2,cc3;
  unsigned long t = millis();
  int frame = 0;
  Sensor& acc;
  Sensor& gyro;
  Sensor& mag;
  BLE_MIDI& midi;
  Madgwick& filter;
  SettingValue& setting;
public:
  RunningStatus(Sensor &acc, Sensor &gyro, Sensor &mag, BLE_MIDI &midi, Madgwick& filter, SettingValue &setting);
  StatusNo exec(bool isFirstPaint);
};