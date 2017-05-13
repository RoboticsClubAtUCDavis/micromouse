#pragma once
#include <Arduino.h>
#undef min
#undef max

#include "Direction.h"
#include "Led.h"
#include "Motor.h"
#include "PIDController.h"
#include "RangeFinder.h"

class Hardware {
  public:
    // PIN DEFINITIONS

    // TODO: set correct pin values
    static const unsigned IRSENSOR_LEFT_PIN = 16;
    static const unsigned IRSENSOR_RIGHT_PIN = 17;
    static const unsigned ULTRASONIC_PIN = 14;

    static const unsigned MOTOR_LEFT_EN_PIN = 3;
    static const unsigned MOTOR_LEFT_IN1_PIN = 7;
    static const unsigned MOTOR_LEFT_IN2_PIN = 6;
    static const unsigned ENCODER_LEFT_A_PIN = 21;
    static const unsigned ENCODER_LEFT_B_PIN = 20;

    static const unsigned MOTOR_RIGHT_EN_PIN = 2;
    static const unsigned MOTOR_RIGHT_IN1_PIN = 5;
    static const unsigned MOTOR_RIGHT_IN2_PIN = 4;
    static const unsigned ENCODER_RIGHT_A_PIN = 23;
    static const unsigned ENCODER_RIGHT_B_PIN = 22;

    static const unsigned DIP_1_PIN = 35;
    static const unsigned DIP_2_PIN = 36;
    static const unsigned DIP_3_PIN = 37;
    static const unsigned DIP_4_PIN = 38;

    static const unsigned BUTTON_PIN = 38;

    static const unsigned LED_PIN = 13;
    // static const unsigned RGB_LED_PIN = 1; // TODO update interface

    static const unsigned NUM_RANGE_FINDERS = 3;
    static const unsigned IRSENSOR_LEFT_MIN_DISTANCE = 20;   // mm
    static const unsigned IRSENSOR_LEFT_MAX_DISTANCE = 150;  // mm
    static const unsigned IRSENSOR_RIGHT_MIN_DISTANCE = 20;  // mm
    static const unsigned IRSENSOR_RIGHT_MAX_DISTANCE = 150; // mm
    static const unsigned ULTRASONIC_MIN_DISTANCE = 20;      // mm
    static const unsigned ULTRASONIC_MAX_DISTANCE = 1000;    // mm

    static const unsigned LEFT_GAP = 40;  // mm
    static const unsigned RIGHT_GAP = 40; // mm

    static const float COUNT_PER_MM;
    static const float MM_PER_COUNT;

    static const unsigned MM_PER_NODE;
    static const unsigned COUNT_PER_NODE;

    Hardware();
    ~Hardware();

    // Moves forward the given number of millimeters.
    // If `keepGoing` is true the mouse will not stop at the end of the
    // movement. If `useCaution` is true it will stop when a wall is
    // encountered. Returns the total millilmeters traveled.
    unsigned moveForward(unsigned mm, bool keepGoing, bool useCaution);

    void rotate(int deg);

    // Eventually we will want movement/turning 'macros' to handle smooth turns
    // and chaining together movements. That we be a ways away.

    // Uses the range finders to detect the presence of a wall.
    void checkWall(Relation relation);

    // Returns the corresponding result after calling `checkWall`.
    bool isWall(Relation relation);

    void setSpeed(unsigned mmps);

    void calibrateMotors();
    void calibrateRangeFinders();
    void calibrateRangeFinder(Relation relation);

    void testMotorSingle();
    void testMotorPair();
    void testMovement();
    void testRotate();
    void testRangeFinderSingle(Relation relation, unsigned iterations = 100,
                               unsigned delay = 50);
    void testRangeFindersAll(unsigned iterations = 100, unsigned delay = 50);

  private:
    void initRangeFinders();

    RangeFinder *rangeFinders[NUM_RANGE_FINDERS];
    Motor leftMotor;
    Motor rightMotor;
    Led led;

    class DeltaTime {
      public:
        DeltaTime() : previousTime(micros()){};
        unsigned operator()();
        void reset();

      private:
        unsigned long previousTime;
    };

    DeltaTime deltaTime;

    PIDController speedPID;
    PIDController distancePID;
    PIDController leftPID;
    PIDController rightPID;

    // Max speed in mmps.
    unsigned speed;
    float secondsPerCount;
};
