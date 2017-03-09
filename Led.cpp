#include "Led.h"

Led::Led() : Pin(Pin) {
}

Led::Led(unsigned pin) : Pin(pin) {
    //	pinMode(pin, OUTPUT);
}

void Led::setBrightness(unsigned percent) {
    //  percent = map(percent, 0, 100, 0, 255);
    //	analogWrite(led, percent);
    brightness = percent;
}

void Led::turnOn() {
    //  analogWrite(led, brightness);
    flag = 1;
}

void Led::turnOff() {
    //	digitalWrite(pin, LOW);
    flag = 0;
}

void Led::blink(unsigned n, unsigned delay1, unsigned delay2) {

    if (flag == 1) {

        for (unsigned i = 0; i <= n; i++) {
            turnOff();
            //		delay(delay1);
            turnOn();
            if (i == n) {
                break;
            }
            //		delay(delay2);
        }

    } else {
        for (unsigned j = 0; j <= n; j++) {
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
