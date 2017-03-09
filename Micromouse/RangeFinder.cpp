#include "RangeFinder.h"
#include "ArduinoSpoof.h"
#include <stdexcept>

RangeFinder::RangeFinder() : DATA_PIN(0), MIN_RANGE(0), MAX_RANGE(0) {
}

RangeFinder::RangeFinder(unsigned pin, unsigned minRange, unsigned maxRange)
    : DATA_PIN(pin), MIN_RANGE(minRange), MAX_RANGE(maxRange) {

    if (DATA_PIN == 0) {
        throw std::invalid_argument("Pin value not set!");
    }

    pinMode(DATA_PIN, INPUT);
}
