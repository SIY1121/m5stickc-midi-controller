#include "accel.h"
#include <M5StickC.h>

bool AccelSensor::init() {
  return M5.Imu.Init() == 0;
}

void AccelSensor::read(float *x, float *y, float *z) {
  M5.Imu.getAccelData(x, y, z);
  *x-=offset.x;
  *y-=offset.y;
  *z-=offset.z;
}

bool AccelSensor::calibrate(float &progress) {
  progress = (millis() - time) / 1000.0f;
  if(time == 0) time = millis();
  float x,y,z;
  M5.Imu.getAccelData(&x, &y, &z);
  calibrate_sum.x+=x;
  calibrate_sum.y+=y;
  calibrate_sum.z+=z;
  count++;
  if(millis() - time < 1000)return false;
  offset.x = calibrate_sum.x / count;
  offset.y = calibrate_sum.y / count;
  offset.z = calibrate_sum.z / count - 1.0; // 重力加速度
  return true;
}