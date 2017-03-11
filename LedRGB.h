#pragma once

class LedRGB {
  public:
    LedRGB();

    const unsigned REDPIN, GREENPIN, BLUEPIN;
    LedRGB(unsigned redPin, unsigned greenPin, unsigned bluePin);
    void setRGB(unsigned r, unsigned g, unsigned b);
    void setBrightness(unsigned percent);
    void turnOn();
    void turnOff();

  private:
    unsigned brightness;
    bool rgbPower;
};
