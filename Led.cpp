#include "Led.h"

Led::Led() {
}

Led::~Led() {
}

Led::Led(unsigned pin)
{
//	pinMode(pin, OUTPUT);
}

void Led::setBrightness(unsigned percent)
{
	unsigned int percent[100];
//	analogWrite(led, precent);
}

void Led::turnOn()
{
//	digitalWrite(pin, HIGH);
}

void Led::turnOff()
{
//	digitalWrite(pin, LOW);
}

void Led::blink(unsigned n = 1, unsigned delay1 = 200, unsigned delay2 = 200)
{
	for (int i; i < n; i++)
	{
		turnOn();
//		delay(delay1);
		turnOff();
		if (i == n)
		{
			break;
		}
//		delay(delay2);		
	}
}

