#include "Arduino.h"

void pinMode(uint8_t pin, uint8_t mode) {
#ifdef VERBOSE_ARDUINO
    printf("pinMode - pin: %u, mode: %u\n", pin, mode);
#endif // VERBOSE_ARDUINO
    mock::teensy.pinMode(pin, mode);
}

void digitalWrite(uint8_t pin, uint8_t val) {
#ifdef VERBOSE_ARDUINO
    printf("digitalWrite - pin: %u, val: %u\n", pin, val);
#endif // VERBOSE_ARDUINO
    mock::teensy.digitalWrite(pin, val);
}

void analogWrite(uint8_t pin, int val) {
#ifdef VERBOSE_ARDUINO
    printf("analogWrite - pin: %u, val: %i\n", pin, val);
#endif // VERBOSE_ARDUINO
    mock::teensy.analogWrite(pin, val);
}

uint8_t digitalRead(uint8_t pin) {
#ifdef VERBOSE_ARDUINO
    printf("digitalRead - pin: %u\n", pin);
#endif // VERBOSE_ARDUINO
    return mock::teensy.digitalRead(pin);
}

int analogRead(uint8_t pin) {
#ifdef VERBOSE_ARDUINO
    printf("analogRead - pin: %u\n", pin);
#endif // VERBOSE_ARDUINO
    return mock::teensy.analogRead(pin);
}

uint32_t millis(void) {
#ifdef VERBOSE_ARDUINO
    printf("millis\n");
#endif // VERBOSE_ARDUINO
    return 0;
}

uint32_t micros(void) {
#ifdef VERBOSE_ARDUINO
    printf("micros\n");
#endif // VERBOSE_ARDUINO
    return 0;
}

void delay(uint32_t msec) {
#ifdef VERBOSE_ARDUINO
    printf("delay - msec: %u\n", msec);
#else
    (void)msec;
#endif // VERBOSE_ARDUINO
}

void delayMicroseconds(uint32_t usec) {
#ifdef VERBOSE_ARDUINO
    printf("delayMicroseconds - pusecin: %u\n", usec);
#else
    (void)usec;
#endif // VERBOSE_ARDUINO
}

uint32_t random(uint32_t howbig) {
#ifdef VERBOSE_ARDUINO
    printf("random - val: %u\n", howbig);
#endif // VERBOSE_ARDUINO
    return rand() % howbig;
}

int32_t random(int32_t howsmall, int32_t howbig) {
#ifdef VERBOSE_ARDUINO
    printf("random - lower: %u, upper: %u\n", howsmall, howbig);
#endif // VERBOSE_ARDUINO
    return rand() % (howbig - howsmall) + howsmall;
}

void randomSeed(uint32_t newseed) {
#ifdef VERBOSE_ARDUINO
    printf("randomSeed - seed: %u\n", newseed);
#endif // VERBOSE_ARDUINO
    srand(newseed);
}

mock::Teensy::Teensy() {
    for (auto i : pins) {
        i = Pin{UNDEFINED, false, false, 0, false};
    }

    // Set analog in pins.
    for (size_t i = 14; i <= 24; i++) {
        pins[i].hasAin = true;
    }
    for (size_t i = 33; i <= 39; i++) {
        pins[i].hasAin = true;
    }

    // Set pwm in pins.
    for (size_t i = 2; i <= 10; i++) {
        pins[i].hasPwm = true;
    }
    pins[14].hasPwm = true;
    pins[16].hasPwm = true;
    pins[17].hasPwm = true;
    for (size_t i = 20; i <= 23; i++) {
        pins[i].hasPwm = true;
    }
}

void mock::Teensy::pinMode(uint8_t pin, uint8_t mode) {
    pins[pin].mode = mode;
}

void mock::Teensy::digitalWrite(uint8_t pin, uint8_t val) {
    Pin &p = pins[pin];

    if (p.mode != OUTPUT) {
        Serial.printf("Waring: Pin %u is not set to OUTPUT\n", pin);
        return;
    }

    p.isHigh = val == HIGH;

    if (p.hasPwm) {
        p.pwm = p.isHigh ? PWM_HIGH : 0;
    }
}

void mock::Teensy::analogWrite(uint8_t pin, int val) {
    Pin &p = pins[pin];

    if (p.mode != OUTPUT) {
        Serial.printf("Waring: Pin %u is not set to OUTPUT\n", pin);
        return;
    }

    if (p.hasPwm) {
        p.pwm = val;
        p.isHigh = val == PWM_HIGH;
    } else {
        Serial.printf("Waring: Pin %u does not support PWM output\n", pin);
        return;
    }
}

uint8_t mock::Teensy::digitalRead(uint8_t pin) {
    Pin &p = pins[pin];

    if (p.mode != INPUT || p.mode != INPUT_PULLUP || p.mode != INPUT_PULLDOWN) {
        Serial.printf("Waring: Pin %u is not set to INPUT\n", pin);
    }

    return 0;
}

int mock::Teensy::analogRead(uint8_t pin) {
    Pin &p = pins[pin];

    if (p.mode != INPUT || p.mode != INPUT_PULLUP || p.mode != INPUT_PULLDOWN) {
        Serial.printf("Waring: Pin %u is not set to INPUT\n", pin);
    }

    if (!p.hasAin) {
        Serial.printf("Waring: Pin %u does not support analog input\n", pin);
    }

    return 0;
}
