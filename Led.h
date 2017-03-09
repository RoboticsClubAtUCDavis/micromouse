#pragma once

class Led {
  public:
    Led();
    ~Led();

	const unsigned pin;
	explicit Led(unsigned pin);
	void setBrightness(unsigned percent);
	void turnOn();
	void turnOff();
	void blink(unsigned n = 1, unsigned delay1 = 200, unsigned delay2 = 200);

  private:
};