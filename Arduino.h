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

class CSerial {
  public:
    int printf(char *fmt, ...) const {
        va_list args;
        va_start(args, fmt);
        int n = vprintf(fmt, args);
        va_end(args);
        return n;
    }
};

static const CSerial Serial;

//#define VERBOSE_ARDUINO

static void pinMode(uint8_t pin, uint8_t mode) {
#ifdef VERBOSE_ARDUINO
    printf("pinMode - pin: %u, mode: %u\n", pin, mode);
#else
    (void)pin;
    (void)mode;
#endif // VERBOSE_ARDUINO
}

static void digitalWrite(uint8_t pin, uint8_t val) {
#ifdef VERBOSE_ARDUINO
    printf("digitalWrite - pin: %u, val: %u\n", pin, val);
#else
    (void)pin;
    (void)val;
#endif // VERBOSE_ARDUINO
}

static void analogWrite(uint8_t pin, int val) {
#ifdef VERBOSE_ARDUINO
    printf("analogWrite - pin: %u, val: %i\n", pin, val);
#else
    (void)pin;
    (void)val;
#endif // VERBOSE_ARDUINO
}

static uint8_t digitalRead(uint8_t pin) {
#ifdef VERBOSE_ARDUINO
    printf("digitalRead - pin: %u\n", pin);
#else
    (void)pin;
#endif // VERBOSE_ARDUINO
    return 0;
}

static int analogRead(uint8_t pin) {
#ifdef VERBOSE_ARDUINO
    printf("analogRead - pin: %u\n", pin);
#else
    (void)pin;
#endif // VERBOSE_ARDUINO
    return 0;
}

static uint32_t millis(void) {
#ifdef VERBOSE_ARDUINO
    printf("millis\n");
#endif // VERBOSE_ARDUINO
    return 0;
}

static uint32_t micros(void) {
#ifdef VERBOSE_ARDUINO
    printf("micros\n");
#endif // VERBOSE_ARDUINO
    return 0;
}

static void delay(uint32_t msec) {
#ifdef VERBOSE_ARDUINO
    printf("delay - msec: %u\n", msec);
#else
    (void)msec;
#endif // VERBOSE_ARDUINO
}

static void delayMicroseconds(uint32_t usec) {
#ifdef VERBOSE_ARDUINO
    printf("delayMicroseconds - pusecin: %u\n", usec);
#else
    (void)usec;
#endif // VERBOSE_ARDUINO
}

static uint32_t random(uint32_t howbig) {
#ifdef VERBOSE_ARDUINO
    printf("random - val: %u\n", howbig);
#endif // VERBOSE_ARDUINO
    return rand() % howbig;
}

static int32_t random(int32_t howsmall, int32_t howbig) {
#ifdef VERBOSE_ARDUINO
    printf("random - lower: %u, upper: %u\n", howsmall, howbig);
#endif // VERBOSE_ARDUINO
    return rand() % (howbig - howsmall) + howsmall;
}

static void randomSeed(uint32_t newseed) {
#ifdef VERBOSE_ARDUINO
    printf("randomSeed - seed: %u\n", newseed);
#endif // VERBOSE_ARDUINO
    srand(newseed);
}

static void srandom(uint32_t newseed) {
#ifdef VERBOSE_ARDUINO
    printf("srandom - seed: %u\n", newseed);
#endif // VERBOSE_ARDUINO
    srand(newseed);
}

#endif