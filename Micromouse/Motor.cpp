#include <Arduino.h>
#undef min
#undef max

#include "Motor.h"
#include <cmath>

Motor::Motor(void) : Motor(0, 0, 0, 0) {
}

Motor::Motor(unsigned en, unsigned ph, unsigned encA, unsigned encB)
    : en(en)
    , ph(ph)
#if defined(__MK66FX1M0__) || defined(__MK20DX256__)
    , encoder(encA, encB) {
#else
{
    // Just so the pins are passed to our mock Teensy
    pinMode(encA, INPUT);
    pinMode(encB, INPUT);
#endif

    pinMode(en, OUTPUT);
    pinMode(ph, OUTPUT);
    off();
}

void Motor::off(void) {
    analogWrite(en, LOW);
    digitalWrite(ph, LOW);
}

// 1.0 = max speed forward; -1.0 = max speed reverse
void Motor::setSpeed(float speed) {
    if (speed < 0.0f) {
        digitalWrite(ph, LOW);
    } else {
        digitalWrite(ph, HIGH);
    }
    analogWrite(en, abs(int(speed * 2047)));
}

long Motor::getCounts(void) {
#if defined(__MK66FX1M0__) || defined(__MK20DX256__)
    long counts = encoder.read();

    if (counts != lastCounts) {
        long time = micros();

        countsPerSecond = (time - lastMicros) /
                          (1E6f /*usec->sec*/ * abs(counts - lastCounts));
        lastCounts = counts;
        lastMicros = time;
    }

    return counts;
#else
    // TODO: May not be the behavior we want
    return 0;
#endif
}
void Motor::resetCounts(void) {
#if defined(__MK66FX1M0__) || defined(__MK20DX256__)
    encoder.write(0);
#endif
}

float Motor::getSecondsPerCount(void) {
    return countsPerSecond;
}

Motor::~Motor() {
    off();
}
