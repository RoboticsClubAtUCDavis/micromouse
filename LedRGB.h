#pragma once
#include "Led.h"

class LedRGB {
  public:
    LedRGB();

    Led RedLed;
    Led GreenLed;
    Led BlueLed;
    LedRGB(unsigned redPin, unsigned greenPin, unsigned bluePin);
    void setRGB(unsigned r, unsigned g, unsigned b);
    void setBrightness(unsigned percent);
    void turnOn();
    void turnOff();

  private:
};
