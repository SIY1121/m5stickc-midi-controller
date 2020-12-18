#include "mag.h"
bool MagSensor::init() {
  Wire.begin(0,26);
  return bmm.initialize() == BMM150_OK;
}

void MagSensor::read(float *x, float *y, float *z) {
  bmm.read_mag_data();
  *x = bmm.mag_data.x + value_offset.x;
  *y = bmm.mag_data.y + value_offset.y;
  *z = bmm.mag_data.z + value_offset.z;
}

bool MagSensor::calibrate() {
  bmm.read_mag_data();

  // 初回呼び出し
  if(time == 0) {
    time = millis();
    value_max = bmm.mag_data;
    value_min = bmm.mag_data;
    return false;
  }

  // 最大値最小値を更新
  value_max.x = max(value_max.x, bmm.mag_data.x);
  value_max.y = max(value_max.y, bmm.mag_data.y);
  value_max.z = max(value_max.z, bmm.mag_data.z);

  value_min.x = min(value_max.x, bmm.mag_data.x);
  value_min.y = min(value_max.y, bmm.mag_data.y);
  value_min.z = min(value_max.z, bmm.mag_data.z);

  // 10秒立つまで以下は実行しない
  if(millis() - time < 10000 )return false;

  // 球の中心位置を計算
  value_offset.x = value_min.x + (value_max.x - value_min.x) / 2;
  value_offset.y = value_min.y + (value_max.y - value_min.y) / 2;
  value_offset.z = value_min.z + (value_max.z - value_min.z) / 2;
  return true;
}