#include <Arduino.h>
#include <M5StickC.h>
#include "sensor/sensor.h"
#include "sensor/mag.h"
#include "sensor/accel.h"
#include "sensor/gyro.h"
#include "ble.h"
#include "status/calibrationReady.h"
#include "status/calibration.h"
#include "status/running.h"
#include "status/setting.h"
#include <MadgwickAHRS.h>

MagSensor mag = MagSensor();
AccelSensor acc = AccelSensor();
GyroSensor gyro = GyroSensor();
BLE_MIDI midi = BLE_MIDI();

Madgwick filter;
SettingValue setting;

Status* statusMap[StatusCount] = {
  new CalibrationReadyStatus(),
  new CalibrationStatus(acc, gyro, mag),
  new RunningStatus(acc, gyro, mag, midi, filter, setting),
  new SettingStatus(acc, gyro, mag, filter, setting),
};

StatusNo currentStatus = StatusNo::CalibrationReady;

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
  M5.Lcd.setTextColor(TFT_WHITE,TFT_DARKGREY);

  filter.begin(100);
}

bool isFirstPaint = true;

void loop()
{
  M5.update();
  StatusNo next = statusMap[currentStatus]->exec(isFirstPaint);
  if(next != currentStatus) {
    currentStatus = next;
    M5.Lcd.fillScreen(TFT_DARKGREY);
    isFirstPaint = true;
  } else {
    isFirstPaint = false;
  }
}