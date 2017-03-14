#include "Motor.h"
#include <Arduino.h>

Motor::Motor(void) : Motor(0, 0, 0) {
}

Motor::Motor(unsigned int en, unsigned int in1, unsigned int in2)
    : en(en), in1(in1), in2(in2) {
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

Motor::~Motor() {
    off();
}
