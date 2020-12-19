#pragma once

#include "sensor/sensor.h"
#include "ble.h"

enum StatusNo {
  CalibrationReady,
  Calibration,
  Running,
  Setting,
  StatusCount
};
class Status {
public:
    virtual StatusNo exec(bool isFirstPaint) = 0;
};