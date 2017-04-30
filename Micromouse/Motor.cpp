#include "Motor.h"
#include <Arduino.h>
#include <cmath>

Motor::Motor(void) : Motor(0, 0, 0, 0, 0) {
}

Motor::Motor(unsigned en, unsigned in1, unsigned in2, unsigned enc1,
             unsigned enc2)
    : en(en)
    , in1(in1)
    , in2(in2)
#if defined(__MK66FX1M0__) || defined(__MK20DX256__)
    , encoder(enc1, enc2)
#endif
{
    (void)enc1;
    (void)enc2;
    pinMode(en, OUTPUT);
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    off();
}

void Motor::off(void) {
    digitalWrite(en, LOW);
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
}

// 1.0 = max speed forward; -1.0 = max speed reverse
void Motor::setSpeed(float speed) {
    if (speed < 0) {
        digitalWrite(in1, LOW);
        digitalWrite(in2, HIGH);
    } else {
        digitalWrite(in2, LOW);
        digitalWrite(in1, HIGH);
    }
    analogWrite(en, abs(int(speed * 255)));
}

long Motor::getCounts(void) {
#if defined(__MK66FX1M0__) || defined(__MK20DX256__)
    long counts = encoder.read();

    if (counts != lastCounts) {
        long time = micros();

        secondsPerCount = (time - lastMicros) /
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
    return secondsPerCount;
}

Motor::~Motor() {
    off();
}
