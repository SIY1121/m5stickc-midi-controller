#ifndef _SETTING_STATUS
#define _SETTING_STATUS

#include "setting.h"
#include "running.h"

SettingStatus::SettingStatus(Sensor &acc, Sensor &gyro, Sensor &mag, Madgwick& filter, SettingValue &setting) : acc(acc), gyro(gyro), mag(mag), filter(filter), setting(setting) {}

StatusNo SettingStatus::exec(bool isFirstPaint) {
  M5.Lcd.setTextColor(TFT_WHITE, TFT_DARKCYAN);

  acc.read(&ax, &ay, &az);
  gyro.read(&gx, &gy, &gz);
  mag.read(&mx, &my, &mz);

  filter.update(gx, gy, gz, ax, ay, az, mx, my, mz);
  
  roll = filter.getRoll();
  pitch = filter.getPitch();
  yaw = filter.getYaw();

  if(!edit) {
    M5.Lcd.drawString(" setting", 2,2,1);
    M5.Lcd.drawFastHLine(0,20,80,TFT_WHITE);
    M5.Lcd.fillTriangle(70,70,78,74,70,78, TFT_WHITE);
    String str = "back";
    for(int i = 0; i < 4; i++)
      M5.Lcd.drawString(String(str[i]), 70, 85 + i * 8, 1);

    M5.Lcd.drawString("select",20,132,2);
    M5.Lcd.fillTriangle(37,150,44,150,40,158,TFT_WHITE);
    if(roll > 10 && millis() - lastTime > 400) {
      selectedIndex = min(3, selectedIndex + 1);
      lastTime = millis();
    } else if(roll < -10 && millis() - lastTime > 400) {
      selectedIndex = max(0, selectedIndex - 1);
      lastTime = millis();
    }
    for(int i = 0; i < 4; i++){
      M5.Lcd.setTextColor(i == selectedIndex ? TFT_GREENYELLOW : TFT_LIGHTGREY, TFT_DARKCYAN);
      M5.Lcd.drawString(menuItem[i], 10, 40 + 20 * i, 2);
    }

    if(M5.BtnA.wasPressed()) {
      M5.Lcd.fillScreen(TFT_DARKCYAN);
      edit = true;
    }
    else if(M5.BtnB.wasPressed())
      return StatusNo::Running;
  }else {
    M5.Lcd.setTextColor(TFT_WHITE, TFT_DARKCYAN);
    M5.Lcd.drawString(" setting", 2,2,1);
    M5.Lcd.drawString("> " + menuItem[selectedIndex], 2, 10, 1);
    M5.Lcd.drawFastHLine(0,20,80,TFT_WHITE);
    M5.Lcd.drawString(menuItem[selectedIndex], 10, 30, 4);

    M5.Lcd.fillTriangle(78,113,72,110,72,116, pitch > 20 ? TFT_GREENYELLOW : TFT_LIGHTGREY);
    M5.Lcd.fillTriangle(2,113,8,110,8,116, pitch < -20 ? TFT_GREENYELLOW : TFT_LIGHTGREY);

    if(pitch > 20 && millis() - lastTime > 200) {
      setting.cc[selectedIndex] = min(127, setting.cc[selectedIndex] + 1);
      lastTime = millis();
    } else if(pitch < -20 && millis() - lastTime > 200) {
      setting.cc[selectedIndex] = max(0, setting.cc[selectedIndex] - 1);
      lastTime = millis();
    }

    M5.Lcd.setCursor(5,65);
    M5.Lcd.setTextFont(4);
    M5.Lcd.printf("CC");

    M5.Lcd.setCursor(20,100);
    M5.Lcd.printf("%u   ", setting.cc[selectedIndex]);

    M5.Lcd.drawString("ok",35,132,2);
    M5.Lcd.fillTriangle(37,150,44,150,40,158,TFT_WHITE);

    if(M5.BtnA.wasPressed()) {
      M5.Lcd.fillScreen(TFT_DARKCYAN);
      edit = false;
    }
  }

  if(millis() - t < 10)
    delay(10 - (millis() - t));
  t = millis();

  return StatusNo::Setting;
}

#endif