#include <Arduino.h>
#undef min
#undef max

#include "RangeFinder.h"
#include <assert.h>

RangeFinder::RangeFinder() : DATA_PIN(0), MIN_RANGE(0), MAX_RANGE(0) {
}

RangeFinder::RangeFinder(unsigned pin, unsigned minRange, unsigned maxRange)
    : DATA_PIN(pin), MIN_RANGE(minRange), MAX_RANGE(maxRange) {

    if (DATA_PIN == 0) {
        Serial.printf("Warning: Pin not set!\n");
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
