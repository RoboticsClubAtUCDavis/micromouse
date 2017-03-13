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
    setColors = true;
    if (!checkBrightness) {
        setBrightness(brightness);
    }
}

void LedRGB::setBrightness(unsigned percent) {
    brightness = percent / 100;
    redBrightness *= brightness;
    greenBrightness *= brightness;
    blueBrightness *= brightness;
    if (setColors) {
        RedLed.setBrightness(redBrightness);
        GreenLed.setBrightness(greenBrightness);
        BlueLed.setBrightness(blueBrightness);
        checkBrightness = true;
    }
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
