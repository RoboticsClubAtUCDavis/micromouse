#pragma once

class RangeFinder {
  public:
    // Pin to read analog data from.
    const int DATA_PIN;
    // Min distance for sensor in mm.
    const int MIN_RANGE;
    // Max distance for sensor in mm.
    const int MAX_RANGE;

    RangeFinder();
    RangeFinder(unsigned pin);

    bool calibrate(/*TODO*/);
    bool loadCalibration(/*TODO*/);
    void saveCalibration(/*TODO*/);

    float getDistance();
};