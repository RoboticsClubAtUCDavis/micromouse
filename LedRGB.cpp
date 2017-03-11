#include "LedRGB.h"
#include "Led.h"

LedRGB::LedRGB() : RedLed(0), GreenLed(0), BlueLed(0) {
}

LedRGB::LedRGB(unsigned redPin, unsigned greenPin, unsigned bluePin)
    : RedLed(redPin), GreenLed(greenPin), BlueLed(bluePin) {
    //	pinMode(redPin, OUTPUT);
    //	pinMode(greenPin, OUTPUT);
    //	pinMode(bluPin, OUTPUT);
}

void LedRGB::setRGB(unsigned r, unsigned g, unsigned b) {
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
