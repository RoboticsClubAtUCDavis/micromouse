#pragma once
#include "Led.h"

class LedRGB {
  public:
    LedRGB();

    LedRGB(unsigned redPin, unsigned greenPin, unsigned bluePin);
    void setRGB(unsigned r = 255, unsigned g = 255, unsigned b = 255);
    void setBrightness(unsigned percent);
    void turnOn();
    void turnOff();

  private:
    Led redLed;
    Led greenLed;
    Led blueLed;
    unsigned redBrightness, greenBrightness, blueBrightness;
    double brightness = 1;
};
