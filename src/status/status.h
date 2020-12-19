#pragma once

#include "sensor/sensor.h"
#include "ble.h"

class Status {
public:
    virtual Status* exec(Sensor& acc, Sensor& gyro, Sensor& mag, BLE_MIDI& midi, bool isFirstPaint) = 0;
};