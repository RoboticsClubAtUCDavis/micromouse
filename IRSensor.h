#pragma once
#include "RangeFinder.h"
class IRSensor : public RangeFinder {
  public:
    IRSensor(){};
    IRSensor(unsigned pin) : RangeFinder(pin){};

    float getDistance();
};
