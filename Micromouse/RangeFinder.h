#pragma once

class RangeFinder {
  public:
    // Pin to read data from.
    const unsigned DATA_PIN;
    // Min distance for sensor in mm.
    const unsigned MIN_RANGE;
    // Max distance for sensor in mm.
    const unsigned MAX_RANGE;

    virtual bool calibrate(/*TODO*/);
    virtual bool loadCalibration(/*TODO*/);
    virtual void saveCalibration(/*TODO*/);

    virtual float getDistance() const = 0;
    virtual ~RangeFinder() = default;

  protected:
    RangeFinder();
    RangeFinder(unsigned pin, unsigned minRange, unsigned maxRange);
};