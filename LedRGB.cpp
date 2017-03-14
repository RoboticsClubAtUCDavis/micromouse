#include "LedRGB.h"
#include "Led.h"

LedRGB::LedRGB() : RedLed(255), GreenLed(255), BlueLed(255) {
}

LedRGB::LedRGB(unsigned redPin, unsigned greenPin, unsigned bluePin)
    : RedLed(redPin), GreenLed(greenPin), BlueLed(bluePin) {
}

void LedRGB::setRGB(unsigned r, unsigned g, unsigned b) {
    redBrightness = r / 2.55;
    greenBrightness = g / 2.55;
    blueBrightness = b / 2.55;
    RedLed.setBrightness(redBrightness * brightness);
    GreenLed.setBrightness(greenBrightness * brightness);
    BlueLed.setBrightness(blueBrightness * brightness);
}

void LedRGB::setBrightness(unsigned percent) {
    brightness = percent / 100;
    RedLed.setBrightness(redBrightness * brightness);
    GreenLed.setBrightness(greenBrightness * brightness);
    BlueLed.setBrightness(blueBrightness * brightness);
}

void LedRGB::turnOn() {
    RedLed.turnOn();
    GreenLed.turnOn();
    BlueLed.turnOn();
}

void LedRGB::turnOff() {
    RedLed.turnOff();
    GreenLed.turnOff();
    BlueLed.turnOff();
}
