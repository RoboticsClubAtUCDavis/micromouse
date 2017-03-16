#include "Led.h"
#include <Arduino.h>

Led::Led() : PIN(0) {
}

Led::Led(unsigned pin) : PIN(pin), brightness(255) {
    pinMode(pin, OUTPUT);
}

void Led::setBrightness(unsigned percent) {
    brightness = percent * 2.55;
    if (ledPower) {
        analogWrite(PIN, brightness);
    }
}

void Led::turnOn() {
    analogWrite(PIN, brightness);
    ledPower = true;
}

void Led::turnOff() {
    digitalWrite(PIN, LOW);
    ledPower = false;
}

void Led::blink(unsigned n, unsigned delay1, unsigned delay2) {

    if (n == 0) {
        return;
    }

    if (ledPower) {

        for (unsigned i = 1;; i++) {
            turnOff();
            delay(delay1);
            turnOn();
            if (i == n) {
                break;
            }
            delay(delay2);
        }

    } else {
        for (unsigned j = 1;; j++) {
            turnOn();
            delay(delay1);
            turnOff();
            if (j == n) {
                break;
            }
            delay(delay2);
        }
    }
}
