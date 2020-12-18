#pragma once

class Sensor {
public:
  virtual bool init() = 0;
  virtual void read(float *x, float *y, float *z) = 0;
  virtual bool calibrate() = 0;
};