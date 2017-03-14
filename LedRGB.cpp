#include "LedRGB.h"
#include "Led.h"

LedRGB::LedRGB() : redLed(0), greenLed(0), blueLed(0) {
}

LedRGB::LedRGB(unsigned redPin, unsigned greenPin, unsigned bluePin)
    : redLed(redPin), greenLed(greenPin), blueLed(bluePin) {
}

void LedRGB::setRGB(unsigned r, unsigned g, unsigned b) {
    redBrightness = r / 2.55;
    greenBrightness = g / 2.55;
    blueBrightness = b / 2.55;
    redLed.setBrightness(redBrightness * brightness);
    greenLed.setBrightness(greenBrightness * brightness);
    blueLed.setBrightness(blueBrightness * brightness);
}

void LedRGB::setBrightness(unsigned percent) {
    brightness = percent / 100;
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

void LedRGB::blink(unsigned n, unsigned delay1, unsigned delay2) {
    redLed.blink(n, delay1, delay2);
    greenLed.blink(n, delay1, delay2);
    blueLed.blink(n, delay1, delay2);
}
