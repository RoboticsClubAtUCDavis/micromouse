#include <Arduino.h>

constexpr auto operator"" _us(unsigned long long int x) {
    return x;
}

constexpr auto operator"" _ms(unsigned long long int x) {
    return x * 1000;
}

constexpr auto operator"" _s(unsigned long long int x) {
    return x * 1000000;
}

constexpr auto operator"" _Hz(unsigned long long int x) {
    return 1000000 / x;
}

constexpr unsigned int ledPin = 2;
constexpr unsigned int detPin = 14;

constexpr unsigned int onDuration = 20000_us;
constexpr unsigned int offDuration = 180_ms;
constexpr unsigned int intervalDuration = 1_s;
constexpr unsigned int blinks = 5;
constexpr unsigned int cycles = 5;
constexpr unsigned int sampleRate = 500_Hz;
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
        Serial.printf("%u,%u,%u\n", 4095, 0, maxInput);

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
