#pragma once
#include "RangeFinder.h"
class UltrasonicSensor : public RangeFinder {
  public:
    UltrasonicSensor(){};
    UltrasonicSensor(unsigned pin) : RangeFinder(pin){};

    float getDistance();
};
