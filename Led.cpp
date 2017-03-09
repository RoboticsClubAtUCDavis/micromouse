#include "Led.h"

Led::Led() : pin(pin) {
}

Led::Led(unsigned pin) : pin(pin) {
    //	pinMode(pin, OUTPUT);
}

void Led::setBrightness(unsigned percent) {
    //	analogWrite(led, precent);
	brightness = percent;
}

void Led::turnOn() {
    //	digitalWrite(pin, HIGH);
}

void Led::turnOff() {
    //	digitalWrite(pin, LOW);
}

void Led::blink(unsigned n, unsigned delay1, unsigned delay2) {
    for (unsigned i = 0; i < n; i++) {
        turnOn();
        //		delay(delay1);
        turnOff();
        if (i == n) {
            break;
        }
        //		delay(delay2);
    }
}
