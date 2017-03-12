#include "LedRGB.h"
#include "Led.h"

LedRGB::LedRGB() : RedLed(0), GreenLed(0), BlueLed(0) {
}

LedRGB::LedRGB(unsigned redPin, unsigned greenPin, unsigned bluePin)
    : RedLed(redPin), GreenLed(greenPin), BlueLed(bluePin) {
}

void LedRGB::setRGB(unsigned r, unsigned g, unsigned b) {
    RedLed.setBrightness(r / 2.55);
    GreenLed.setBrightness(g / 2.55);
    BlueLed.setBrightness(b / 2.55);
}

void LedRGB::setBrightness(unsigned percent) {
    RedLed.setBrightness(percent);
    GreenLed.setBrightness(percent);
    BlueLed.setBrightness(percent);
}

void LedRGB::turnOn() {
    RedLed.turnOn();
    GreenLed.turnOn();
    BlueLed.turnOn();
}

void LedRGB::turnOff() {
    RedLed.turnOn();
    GreenLed.turnOn();
    BlueLed.turnOff();
}
