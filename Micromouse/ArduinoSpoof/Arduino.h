#ifndef ARDUINO_SPOOF
#define ARDUINO_SPOOF

#include <cstdarg>
#include <cstdlib>
#include <stdint.h>
#include <stdio.h>

#define HIGH 1
#define LOW 0
#define INPUT 0
#define OUTPUT 1
#define INPUT_PULLUP 2
#define INPUT_PULLDOWN 3
#define OUTPUT_OPENDRAIN 4
#define INPUT_DISABLE 5
#define UNDEFINED 6

namespace mock {
class CSerial {
  public:
    int printf(const char *fmt, ...) const {
        va_list args;
        va_start(args, fmt);
        int n = vprintf(fmt, args);
        va_end(args);
        return n;
    }
};

class Teensy {
  public:
    struct Pin {
        uint8_t mode;
        bool isHigh;
        bool hasPwm;
        int pwm;
        bool hasAin;
    };

    static const int PWM_HIGH = 255;

    Teensy();

    void pinMode(uint8_t pin, uint8_t mode);
    void digitalWrite(uint8_t pin, uint8_t val);
    void analogWrite(uint8_t pin, int val);
    uint8_t digitalRead(uint8_t pin);
    int analogRead(uint8_t pin);

    Pin pins[40];
};

extern Teensy teensy;
}

static const mock::CSerial Serial;

//#define VERBOSE_ARDUINO

void pinMode(uint8_t pin, uint8_t mode);
void digitalWrite(uint8_t pin, uint8_t val);
void analogWrite(uint8_t pin, int val);
uint8_t digitalRead(uint8_t pin);
int analogRead(uint8_t pin);
uint32_t millis(void);
uint32_t micros(void);
void delay(uint32_t msec);
void delayMicroseconds(uint32_t usec);
uint32_t random(uint32_t howbig);
int32_t random(int32_t howsmall, int32_t howbig);
void randomSeed(uint32_t newseed);

#endif
