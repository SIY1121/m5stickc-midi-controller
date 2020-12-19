#include "sensor.h"
#include <bmm150.h>
struct MagData
{
    int16_t x = 0;
    int16_t y = 0;
    int16_t z = 0;
};
class MagSensor: public Sensor {
private:
  BMM150 bmm;
  MagData value_offset;
  MagData value_max;
  MagData value_min;
  unsigned long time = 0;
public:
  bool init();
  void read(float *x, float *y, float *z);
  bool calibrate(float &progress);
};