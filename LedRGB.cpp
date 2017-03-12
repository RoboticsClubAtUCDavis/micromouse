#include "LedRGB.h"
#include "Led.h"

LedRGB::LedRGB() : RedLed(0), GreenLed(0), BlueLed(0) {
}

LedRGB::LedRGB(unsigned redPin, unsigned greenPin, unsigned bluePin)
    : RedLed(redPin), GreenLed(greenPin), BlueLed(bluePin) {
}

void LedRGB::setRGB(unsigned r, unsigned g, unsigned b) {
    r /= 2.55;
    g /= 2.55;
    b /= 2.55;
    if (checkBrightness) {
        RedLed.setBrightness(r);
        GreenLed.setBrightness(g);
        BlueLed.setBrightness(b);
    }
}

void LedRGB::setBrightness(unsigned percent) {
    RedLed.setBrightness(percent);
    GreenLed.setBrightness(percent);
    BlueLed.setBrightness(percent);
    checkBrightness = true;
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
