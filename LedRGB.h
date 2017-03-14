#pragma once
#include "Led.h"

class LedRGB {
  public:
    LedRGB();

    LedRGB(unsigned redPin, unsigned greenPin, unsigned bluePin);
    void setRGB(unsigned r, unsigned g, unsigned);
    void setBrightness(unsigned percent);
    void turnOn();
    void turnOff();

  private:
    Led redLed;
    Led greenLed;
    Led blueLed;
    unsigned redBrightness = 100, greenBrightness = 100, blueBrightness = 100;
    double brightness = 1;
};
