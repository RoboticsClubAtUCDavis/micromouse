#include "Led.h"

Led::Led() : PIN(0) {
}

Led::Led(unsigned pin) : PIN(pin) {
    //	pinMode(pin, OUTPUT);
}

void Led::setBrightness(unsigned percent) {
    percent *= 2.55;
    //	analogWrite(led, percent);
    brightness = percent;
}

void Led::turnOn() {
    //  analogWrite(led, brightness);
    flag = true;
}

void Led::turnOff() {
    //	digitalWrite(pin, LOW);
    flag = false;
}

void Led::blink(unsigned n, unsigned delay1, unsigned delay2) {

    if (flag == true) {

        for (unsigned i = 1; i <= n; i++) {
            turnOff();
            //		delay(delay1);
            turnOn();
            if (i == n) {
                break;
            }
            //		delay(delay2);
        }

    } else {
        for (unsigned j = 1; j <= n; j++) {
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
