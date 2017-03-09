#pragma once
#include "RangeFinder.h"
class UltraSonicSensor : public RangeFinder {
  public:
    UltraSonicSensor(){};
    UltraSonicSensor(unsigned pin) : RangeFinder(pin){};

    float getDistance();
};
