#pragma once
#include <iostream>
#include <stdint.h>

#define HIGH 1
#define LOW 0
#define INPUT 0
#define OUTPUT 1
#define INPUT_PULLUP 2
#define INPUT_PULLDOWN 3
#define OUTPUT_OPENDRAIN 4
#define INPUT_DISABLE 5

//#define VERBOSE_ARDUINO

#ifdef VERBOSE_ARDUINO
#define verbose_print(str) std::cout << str << std::endl;
#else
#define verbose_print(str)
#endif // VERBOSE_ARDUINO

void pinMode(uint8_t pin, uint8_t mode) {
#ifdef VERBOSE_ARDUINO
    printf("pinMode - pin: %u, mode: %u\n", pin, mode);
#endif // VERBOSE_ARDUINO
}

void digitalWrite(uint8_t pin, uint8_t val) {
#ifdef VERBOSE_ARDUINO
    printf("digitalWrite - pin: %u, val: %u\n", pin, val);
#endif // VERBOSE_ARDUINO
}

void analogWrite(uint8_t pin, int val) {
#ifdef VERBOSE_ARDUINO
    printf("analogWrite - pin: %u, val: %i\n", pin, val);
#endif // VERBOSE_ARDUINO
}

uint8_t digitalRead(uint8_t pin) {
#ifdef VERBOSE_ARDUINO
    printf("digitalRead - pin: %u\n", pin);
#endif // VERBOSE_ARDUINO
    return 0;
}

int analogRead(uint8_t pin) {
#ifdef VERBOSE_ARDUINO
    printf("analogRead - pin: %u\n", pin);
#endif // VERBOSE_ARDUINO
    return 0;
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
#endif // VERBOSE_ARDUINO
}

void delayMicroseconds(uint32_t usec) {
#ifdef VERBOSE_ARDUINO
    printf("delayMicroseconds - pusecin: %u\n", usec);
#endif // VERBOSE_ARDUINO
}
