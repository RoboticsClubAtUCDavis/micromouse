#pragma once
#include "RangeFinder.h"
class UltrasonicSensor : public RangeFinder {
  public:
    UltrasonicSensor(){};
    UltrasonicSensor(unsigned pin, unsigned minRange, unsigned maxRange)
        : RangeFinder(pin, minRange, maxRange){};
    ~UltrasonicSensor() = default;

    float getDistance();
};
