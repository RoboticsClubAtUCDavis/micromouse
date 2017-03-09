#pragma once

class RangeFinder {
  public:
    // Pin to read analog data from.
    const int DATA_PIN;
    // Min distance for sensor in mm.
    const int MIN_RANGE;
    // Max distance for sensor in mm.
    const int MAX_RANGE;

    // These may need to be virtual.
    // Going to try to avoid it.
    bool calibrate(/*TODO*/);
    bool loadCalibration(/*TODO*/);
    void saveCalibration(/*TODO*/);

    virtual float getDistance() = 0;

  protected:
    RangeFinder();
    RangeFinder(unsigned pin);
};