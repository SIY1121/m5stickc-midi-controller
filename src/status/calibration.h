#pragma once

#include "status.h"
#include "M5StickC.h"
#include "status/running.h"
#include "sensor/sensor.h"

class CalibrationStatus: public Status {
  uint8_t step = 1;
  Sensor &acc;
  Sensor &gyro;
  Sensor &mag;
public:
  CalibrationStatus(Sensor &acc,  Sensor &gyro,Sensor &mag);
  StatusNo exec(bool isFirstPaint);
};

CalibrationStatus::CalibrationStatus(Sensor &acc,  Sensor &gyro,Sensor &mag): acc(acc), gyro(gyro), mag(mag){}

StatusNo CalibrationStatus::exec(bool isFirstPaint) {
  if(step == 1) {
    float aProgress, gProgress;
    bool aResult, gResult;
    aResult = acc.calibrate(aProgress);
    gResult = gyro.calibrate(gProgress);

    M5.Lcd.setCursor(5,100);
    M5.Lcd.setTextDatum(4);
    M5.Lcd.drawString("DO NOT", 40, 90, 2);
    M5.Lcd.drawString("Move", 40, 110, 2);
    M5.Lcd.fillRect(5,125,70, 5, TFT_BLACK);
    M5.Lcd.fillRect(5,125,70 * (1 - (aProgress + gProgress) / 2), 5, TFT_WHITE);
    if(aResult && gResult) step = 2;
  } else if(step == 2) {
    float progress;
    M5.Lcd.setCursor(5,100);
    M5.Lcd.setTextDatum(4);
    M5.Lcd.drawString("Rotate", 40, 90, 2);
    M5.Lcd.drawString("      ", 40, 110, 2);

    if(mag.calibrate(progress))return StatusNo::Running;
    else {
      M5.Lcd.fillRect(5,125,70, 5, TFT_BLACK);
      M5.Lcd.fillRect(5,125,70 * (1 - progress), 5, TFT_WHITE);
    }
  }
  delay(2);
  return StatusNo::Calibration;
}