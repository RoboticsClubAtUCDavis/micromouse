#include "LedRGB.h"
#include "Led.h"
#include <stdint.h>	

LedRGB::LedRGB() : redLed(0), greenLed(0), blueLed(0) {
}

LedRGB::LedRGB(uint8_t redPin, uint8_t greenPin, uint8_t bluePin)
    : redLed(redPin), greenLed(greenPin), blueLed(bluePin) {
}

void LedRGB::setRGB(uint8_t r, uint8_t g, uint8_t b) {
    redBrightness = r / 255.0f;
    greenBrightness = g / 255.0f;
    blueBrightness = b / 255.0f;
    redLed.setBrightness(redBrightness * brightness);
    greenLed.setBrightness(greenBrightness * brightness);
    blueLed.setBrightness(blueBrightness * brightness);
}

void LedRGB::setBrightness(float percent) {
    brightness = percent;
    redLed.setBrightness(redBrightness * brightness);
    greenLed.setBrightness(greenBrightness * brightness);
    blueLed.setBrightness(blueBrightness * brightness);
}

void LedRGB::turnOn() {
    redLed.turnOn();
    greenLed.turnOn();
    blueLed.turnOn();
}

void LedRGB::turnOff() {
    redLed.turnOff();
    greenLed.turnOff();
    blueLed.turnOff();
}
