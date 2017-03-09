#pragma once
#include "RangeFinder.h"
class IRSensor : public RangeFinder {
  public:
    IRSensor(){};
    IRSensor(unsigned pin, unsigned minRange, unsigned maxRange)
        : RangeFinder(pin, minRange, maxRange){};
    ~IRSensor() = default;

    float getDistance();
};
