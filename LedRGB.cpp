#include "LedRGB.h"
#include "Led.h"

LedRGB::LedRGB() : RedLed(0), GreenLed(0), BlueLed(0) {
}

LedRGB::LedRGB(unsigned redPin, unsigned greenPin, unsigned bluePin)
    : RedLed(redPin), GreenLed(greenPin), BlueLed(bluePin) {
}

void LedRGB::setRGB(unsigned r, unsigned g, unsigned b) {
    redBrightness = r / 2.55;
    greenBrightness = g / 2.55;
    blueBrightness = b / 2.55;
    RedLed.setBrightness(redBrightness);
    GreenLed.setBrightness(greenBrightness);
    BlueLed.setBrightness(blueBrightness);
}

void LedRGB::setBrightness(unsigned percent) {
    percent /= 100;
    RedLed.setBrightness(redBrightness * percent);
    GreenLed.setBrightness(greenBrightness * percent);
    BlueLed.setBrightness(blueBrightness * percent);
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
