#pragma once

class RangeFinder {
  public:
    // Pin to read data from.
    const unsigned DATA_PIN;
    // Min distance for sensor in mm.
    const unsigned MIN_RANGE;
    // Max distance for sensor in mm.
    const unsigned MAX_RANGE;

    // These may need to be virtual.
    // Going to try to avoid it.
    bool calibrate(/*TODO*/);
    bool loadCalibration(/*TODO*/);
    void saveCalibration(/*TODO*/);

    virtual float getDistance() = 0;
    virtual ~RangeFinder() = default;

  protected:
    RangeFinder();
    RangeFinder(unsigned pin, unsigned minRange, unsigned maxRange);
};