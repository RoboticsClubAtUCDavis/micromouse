#pragma once
#include "RangeFinder.h"
class IRSensor : public RangeFinder {
  public:
    IRSensor(){};
    IRSensor(unsigned pin, unsigned minRange, unsigned maxRange)
        : RangeFinder(pin, minRange, maxRange){};
    ~IRSensor() = default;

    float getDistance() const;
    bool calibrate();
    bool loadCalibration();

  private:
    unsigned readSensor(unsigned samples = 1, unsigned delay = 40) const;
    float calcDistance(unsigned sensorVal) const;

    float a, b, c;
    unsigned lowerBound, upperBound;
};
