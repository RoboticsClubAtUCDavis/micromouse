#include "Led.h"

Led::Led() : PIN(0) {
}

Led::Led(unsigned pin) : PIN(pin) {
    //	pinMode(pin, OUTPUT);
}

void Led::setBrightness(unsigned percent) {
    if (ledPower) {
        brightness = percent *= 2.55;
    } else {
        brightness = percent *= 2.55;
        //	analogWrite(led, brightness);
    }
}

void Led::turnOn() {
    //  analogWrite(led, brightness);
    ledPower = true;
}

void Led::turnOff() {
    //	digitalWrite(pin, LOW);
    ledPower = false;
}

void Led::blink(unsigned n, unsigned delay1, unsigned delay2) {

    if (ledPower) {

        for (unsigned i = 1;; i++) {
            turnOff();
            //		delay(delay1);
            turnOn();
            if (i == n) {
                break;
            }
            //		delay(delay2);
        }

    } else {
        for (unsigned j = 1;; j++) {
            turnOn();
            //		delay(delay1);
            turnOff();
            if (j == n) {
                break;
            }
            //		delay(delay2);
        }
    }
}
