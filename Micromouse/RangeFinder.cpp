#include "RangeFinder.h"
#include "ArduinoSpoof.h"
#include <stdexcept>

RangeFinder::RangeFinder() : DATA_PIN(0), MIN_RANGE(0), MAX_RANGE(0) {
}

RangeFinder::RangeFinder(unsigned pin, unsigned minRange, unsigned maxRange)
    : DATA_PIN(pin), MIN_RANGE(minRange), MAX_RANGE(maxRange) {

    if (DATA_PIN == 0) {
        // So Teensy/Arduino doesn't support exceptions
        // We should implement some way to log errors
        // throw std::invalid_argument("Pin value not set!");
        std::cout << "Warning: Pin not set" << std::endl;
        return;
    }

    pinMode(DATA_PIN, INPUT);
}

bool RangeFinder::calibrate() {
    // TODO
    return false;
}

bool RangeFinder::loadCalibration() {
    // TODO
    return false;
}

void RangeFinder::saveCalibration() {
    // TODO
}
