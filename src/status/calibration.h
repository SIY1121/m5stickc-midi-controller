#include "status.h"
#include "M5StickC.h"
#include "status/running.h"


class Calibration: public Status {
  uint8_t step = 1;
public:
  Status* exec(Sensor& acc, Sensor& gyro, Sensor& mag, BLE_MIDI& midi, bool isFirstPaint);
};

Status *calibrationStatus = new Calibration();

Status* Calibration::exec(Sensor& acc, Sensor& gyro, Sensor& mag, BLE_MIDI& midi, bool isFirstPaint) {
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

    if(mag.calibrate(progress))return runningStatus;
    else {
      M5.Lcd.fillRect(5,125,70, 5, TFT_BLACK);
      M5.Lcd.fillRect(5,125,70 * (1 - progress), 5, TFT_WHITE);
    }
  }
  delay(2);
  return calibrationStatus;
}