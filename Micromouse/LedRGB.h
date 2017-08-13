#pragma once
#include "Led.h"
#include <stdint.h>

class LedRGB : public Led {
  public:
    LedRGB();

    LedRGB(uint8_t redPin, uint8_t greenPin, uint8_t bluePin);
    void setRGB(uint8_t r, uint8_t g, uint8_t b);
    void setBrightness(float percent);
    void turnOn();
    void turnOff();

  private:
    Led redLed;
    Led greenLed;
    Led blueLed;
    float redBrightness = 1.0f, greenBrightness = 1.0f, blueBrightness = 1.0f;
    float brightness = 1.0f;
};
