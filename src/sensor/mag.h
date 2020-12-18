#include "sensor.h"
#include <bmm150.h>

class MagSensor: public Sensor {
private:
  BMM150 bmm;
  bmm150_mag_data value_offset;
  bmm150_mag_data value_max;
  bmm150_mag_data value_min;
  unsigned long time = 0;
public:
  bool init();
  void read(float *x, float *y, float *z);
  bool calibrate();
};