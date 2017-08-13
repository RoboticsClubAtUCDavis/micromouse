#pragma once

class Led {
  public:
    Led();

    const unsigned PIN;
    explicit Led(unsigned pin);
    virtual void setBrightness(float percent);
    virtual void turnOn();
    virtual void turnOff();
    void blink(unsigned n = 1, unsigned delay1 = 200, unsigned delay2 = 200);

  private:
    float brightness;
    bool ledPower;
};
