#include "Micromouse/Hardware.h"
#include "Micromouse/Led.h"
#include "catch.hpp"
#include <Arduino.h>
using namespace mock;

SCENARIO("Leds can be fully controlled", "[led]") {
    GIVEN("An Led with a pin") {
        const unsigned pinNum = 2;
        Teensy::Pin &pin = teensy.pins[pinNum];
        teensy.analogWriteResolution(Hardware::WRITE_RESOLUTION);

        REQUIRE(pin.hasPwm);

        Led led(pinNum);

        CHECK(pin.mode == OUTPUT);

        THEN("The led starts off") {
            CHECK(pin.pwm == 0);
            CHECK_FALSE(pin.isHigh);
        }

        WHEN("The Led is turned on") {
            led.turnOn();
            THEN("The pin is set high") {
                CHECK(pin.isHigh);
                CHECK(pin.pwm == Teensy::PWM_HIGH);
            }

            WHEN("The Led is turned off") {
                led.turnOff();
                THEN("The pin is set low") {
                    CHECK_FALSE(pin.isHigh);
                    CHECK(pin.pwm == 0);
                }

                WHEN("The brighntess is set once") {
                    float b = 0.50f;
                    led.setBrightness(b);
                    THEN("The pin pwm is not changed immediately") {
                        CHECK_FALSE(pin.isHigh);
                        CHECK(pin.pwm == 0);
                    }

                    WHEN("The Led is turned on") {
                        led.turnOn();
                        THEN("The pin pwm is now set correctly based on set "
                             "brightness") {
                            CHECK(pin.pwm == unsigned(b * Teensy::PWM_HIGH));
                        }
                    }
                }

                WHEN("The brighntess is set multiple times") {
                    float b = 0.50f;
                    led.setBrightness(b);
                    led.setBrightness(b);
                    led.setBrightness(b);
                    led.setBrightness(b);
                    THEN("The pin pwm is not changed immediately") {
                        CHECK_FALSE(pin.isHigh);
                        CHECK(pin.pwm == 0);
                    }
                    WHEN("The Led is turned on") {
                        led.turnOn();
                        THEN("The pin pwm is now set correctly based on set "
                             "brightness") {
                            CHECK(pin.pwm == unsigned(b * Teensy::PWM_HIGH));
                        }
                    }
                }

                WHEN("The Led blinks") {
                    float b = 0.50f;
                    led.setBrightness(b);
                    led.blink(3);
                    THEN("After the blink it i returns to the same state") {
                        CHECK(pin.pwm == 0);
                        led.turnOn();
                        CHECK(pin.pwm == unsigned(b * Teensy::PWM_HIGH));
                    }
                    led.blink(0);
                    THEN("After the blink it i returns to the same state") {
                        CHECK(pin.pwm == 0);
                        led.turnOn();
                        CHECK(pin.pwm == unsigned(b * Teensy::PWM_HIGH));
                    }
                }
            }

            WHEN("The brighntess is set once") {
                float b = 0.50f;
                led.setBrightness(b);
                THEN("The pin pwm is set correctly") {
                    CHECK(pin.pwm == unsigned(b * Teensy::PWM_HIGH));
                }
            }

            WHEN("The brighntess is set multiple times") {
                float b = 0.50f;
                led.setBrightness(b);
                led.setBrightness(b);
                led.setBrightness(b);
                led.setBrightness(b);
                THEN("The brightness is not compounded") {
                    CHECK(pin.pwm == unsigned(b * Teensy::PWM_HIGH));
                }
            }

            WHEN("The Led blinks") {
                float b = 0.50f;
                led.setBrightness(b);
                led.blink(3);
                THEN("After the blink it i returns to the same state") {
                    CHECK(pin.pwm == unsigned(b * Teensy::PWM_HIGH));
                }
                led.blink(0);
                THEN("After the blink it i returns to the same state") {
                    CHECK(pin.pwm == unsigned(b * Teensy::PWM_HIGH));
                }
            }
        }
    }
}