#ifndef _RUNNING_STATUS
#define _RUNNING_STATUS
#include "running.h"

RunningStatus::RunningStatus(Sensor &acc, Sensor &gyro, Sensor &mag, BLE_MIDI &midi, Madgwick& filter, SettingValue &setting): acc(acc), gyro(gyro), mag(mag), midi(midi), filter(filter), setting(setting) {}

uint8_t degToValue(const float &deg) {
  return (sin(deg / 180.0 * PI) + 1.0) / 2.0 * 127.0;
}

StatusNo RunningStatus::exec(bool isFirstPaint) {

  if(isFirstPaint) {
    M5.Lcd.setTextColor(TFT_WHITE, TFT_DARKGREY);
    M5.Lcd.setTextDatum(0);
    M5.Lcd.setTextSize(1);
    M5.Lcd.setTextFont(1);
    M5.Lcd.drawFastHLine(0,20,80,TFT_WHITE);
    M5.Lcd.setCursor(6, 29);
    M5.Lcd.printf("roll CC%u", setting.cc[0]);
    M5.Lcd.setCursor(6, 74);
    M5.Lcd.printf("pitch CC%u", setting.cc[1]);
    M5.Lcd.setCursor(6, 119);
    M5.Lcd.printf("yaw CC%u", setting.cc[2]);
    M5.Lcd.fillTriangle(70,70,78,74,70,78, TFT_WHITE);
    String str = "setting";
    for(int i = 0; i < 7; i++)
      M5.Lcd.drawString(String(str[i]), 70, 85 + i * 8, 1);
  }

  if(midi.isConnected()){
    M5.Lcd.drawString("BLE connected", 2,2,1);
  }
  else {
    M5.Lcd.setTextColor(TFT_WHITE, TFT_RED);
    M5.Lcd.drawString("BLE adv...   ", 2,2,1);
  }
  M5.Lcd.setTextColor(TFT_WHITE, TFT_DARKGREY);

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

  M5.Lcd.setTextSize(1);
  M5.Lcd.setTextFont(4);
  M5.Lcd.setCursor(6, 40);
  M5.Lcd.printf("%u   ", cc1);
  M5.Lcd.setCursor(6, 85);
  M5.Lcd.printf("%u   ", cc2);
  M5.Lcd.setCursor(6, 130);
  M5.Lcd.printf("%u   ", cc3);
  

  if(midi.isConnected()) {
    if(frame == 2) {
      midi.control(0, setting.cc[0], cc1);
      midi.control(0, setting.cc[1], cc2);
      midi.control(0, setting.cc[2], cc3);
      frame = 0;
    } else frame++;

    if(M5.BtnA.wasPressed())
      midi.control(0,setting.cc[3],127);
    else if(M5.BtnA.wasReleased())
      midi.control(0,setting.cc[3],0);
  }

  if(millis() - t < 10)
    delay(10 - (millis() - t));
  t = millis();

  if(M5.BtnB.wasPressed())
    return StatusNo::Setting;

  return StatusNo::Running;
}

#endif