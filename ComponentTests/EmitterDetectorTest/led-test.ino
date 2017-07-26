#include <Arduino.h>

constexpr auto operator"" _us(unsigned long long int x) {
    return x;
}

constexpr unsigned us = 1;
constexpr unsigned ms = 1E3;
constexpr unsigned s = 1E6;

constexpr unsigned int ledPin = 2;
constexpr unsigned int detPin = 14;

constexpr unsigned int onDuration = 20000 * us;
constexpr unsigned int offDuration = 180 * ms;
constexpr unsigned int intervalDuration = 1 * s;
constexpr unsigned int blinks = 5;
constexpr unsigned int cycles = 5;
constexpr unsigned int sampleRate = 1000000 / 500 /*Hz*/;
constexpr unsigned int readResolution = 12;
constexpr unsigned int maxInput = (1 << readResolution) - 1;

IntervalTimer sampleTimer;

void blink(unsigned n);
void read();

int main() {

    Serial.begin(115200);

    pinMode(13, OUTPUT);
    digitalWrite(13, HIGH);

    pinMode(ledPin, OUTPUT);
    pinMode(detPin, INPUT);

    analogReadResolution(readResolution);

    delay(2000);

    for (unsigned i = 1;; i++) {
        Serial.printf("%u,%u,%u\n", maxInput, 0, maxInput); // Visual indicator

        sampleTimer.begin(read, sampleRate);
        blink(blinks);
        if (i >= cycles) {
            break;
        }
        sampleTimer.end();
        delayMicroseconds(intervalDuration);
    }

    sampleTimer.end();
}

void blink(unsigned n) {
    for (unsigned i = 1;; i++) {
        digitalWrite(ledPin, HIGH);
        delayMicroseconds(onDuration);
        digitalWrite(ledPin, LOW);
        if (i >= n) {
            break;
        }
        delayMicroseconds(offDuration);
    }
}

void read() {
    Serial.printf("%u,%u,%u\n", analogRead(detPin), 0, maxInput);
}
