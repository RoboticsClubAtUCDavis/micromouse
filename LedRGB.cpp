#include "LedRGB.h"

LedRGB::LedRGB() : REDPIN(0), GREENPIN(0), BLUEPIN(0) {
}

LedRGB::LedRGB(unsigned redPin, unsigned greenPin, unsigned bluePin)
    : REDPIN(redPin), GREENPIN(greenPin), BLUEPIN(bluePin) {
    //	pinMode(redPin, OUTPUT);
    //	pinMode(greenPin, OUTPUT);
    //	pinMode(bluPin, OUTPUT);
}

void LedRGB::setRGB(unsigned r, unsigned g, unsigned b) {
    r = 255 - r;
    g = 255 - g;
    b = 255 - b;

    //	analogWrite(redPin, r);
    //	analogWrite(greenPin, g);
    //	analogWrite(bluePin, b);
}

void LedRGB::setBrightness(unsigned percent) {
    if (rgbPower) {
        brightness = percent * 2.55;
        //	analogWrite(led, brightness);
    }
}

void LedRGB::turnOn() {
    //  analogWrite(led, brightness);
    rgbPower = true;
}

void LedRGB::turnOff() {
    //	digitalWrite(pin, LOW);
    rgbPower = false;
}
