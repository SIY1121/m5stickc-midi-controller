#include "gyro.h"
#include <M5StickC.h>

bool GyroSensor::init() {
  return M5.Imu.Init() == 0;
}

void GyroSensor::read(float *x, float *y, float *z) {
  M5.Imu.getGyroData(x, y, z);
  *x-=offset.x;
  *y-=offset.y;
  *z-=offset.z;
}

bool GyroSensor::calibrate() {
  delay(2);
  if(time == 0) time = millis();
  float x,y,z;
  M5.Imu.getGyroData(&x, &y, &z);
  calibrate_sum.x+=x;
  calibrate_sum.y+=y;
  calibrate_sum.z+=z;
  count++;
  if(millis() - time < 1000)return false;
  offset.x = calibrate_sum.x / count;
  offset.y = calibrate_sum.y / count;
  offset.z = calibrate_sum.z / count;
  return true;
}