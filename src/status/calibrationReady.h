#pragma once

#include "status.h"
#include "M5StickC.h"
#include "status/Calibration.h"
class CalibrationReadyStatus: public Status {
public:
  StatusNo exec(bool isFirstPaint);
};

StatusNo CalibrationReadyStatus::exec(bool isFirstPaint) {
  if(isFirstPaint){
    M5.Lcd.setCursor(0,90);
    M5.Lcd.setTextDatum(4);
    M5.Lcd.drawString("start", 40, 90, 4);
    M5.Lcd.drawString("cal", 40, 115, 4);
    M5.Lcd.fillTriangle(30,130,50,130,40,150, TFT_WHITE);
  }
  if(M5.BtnA.wasPressed())
    return StatusNo::Calibration;
  else
    return StatusNo::CalibrationReady;
}
