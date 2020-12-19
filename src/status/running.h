#include "status.h"
#include "M5StickC.h"
#include <MadgwickAHRS.h>

class Running: public Status {
  Madgwick filter;
  float ax, ay, az, gx, gy, gz, mx, my, mz;
  float roll, pitch, yaw;
  uint8_t cc1,cc2,cc3;
  unsigned long t = millis();
  int frame = 0;
public:
  Running();
  Status* exec(Sensor& acc, Sensor& gyro, Sensor& mag, BLE_MIDI& midi, bool isFirstPaint);
};

Status *runningStatus = new Running();

Running::Running() {
  filter.begin(100);
}

uint8_t degToValue(const float &deg) {
  return (sin(deg / 180.0 * PI) + 1.0) / 2.0 * 127.0;
}

Status* Running::exec(Sensor& acc, Sensor& gyro, Sensor& mag, BLE_MIDI& midi, bool isFirstPaint) {

  if(isFirstPaint) {
    M5.Lcd.setTextDatum(0);
    M5.Lcd.setTextSize(1);
    M5.Lcd.drawString("r CC1", 6, 22, 1);
    M5.Lcd.drawString("p CC2", 6, 70, 1);
    M5.Lcd.drawString("y CC3", 6, 117, 1);
    M5.Lcd.fillTriangle(70,70,78,74,70,78, TFT_WHITE);
    String str = "setting";
    for(int i = 0; i < 7; i++)
      M5.Lcd.drawString(String(str[i]), 70, 85 + i * 8, 1);
  }

  acc.read(&ax, &ay, &az);
  gyro.read(&gx, &gy, &gz);
  mag.read(&mx, &my, &mz);

  filter.update(gx, gy, gz, ax, ay, az, mx, my, mz);
  
  roll = filter.getRoll();
  pitch = filter.getPitch();
  yaw = filter.getYaw();

  cc1 = degToValue(roll);
  cc2 = degToValue(pitch);
  cc3 = degToValue(yaw);

  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextFont(2);
  M5.Lcd.setCursor(6, 33);
  M5.Lcd.printf("%u ", cc1);
  M5.Lcd.setCursor(6, 80);
  M5.Lcd.printf("%u ", cc2);
  M5.Lcd.setCursor(6, 127);
  M5.Lcd.printf("%u ", cc3);
  

  if(midi.isConnected()) {
    if(frame == 2) {
      midi.control(0, 1, cc1);
      midi.control(0, 2, cc2);
      midi.control(0, 3, cc3);
      frame = 0;
    } else frame++;

    if(M5.BtnA.wasPressed())
      midi.control(0,4,127);
    else if(M5.BtnA.wasReleased())
      midi.control(0,4,0);
  }

  if(millis() - t < 10)
    delay(10 - (millis() - t));
  t = millis();

  return runningStatus;
}