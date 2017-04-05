#pragma once
#include "Direction.h"
#include "Led.h"
#include "Motor.h"
#include "PIDController.h"
#include "RangeFinder.h"
#include <Arduino.h>

enum Relation { LEFT, FRONT, RIGHT, NO_RELATION };

class Hardware {
  public:
    // PIN DEFINITIONS

    // TODO: set correct pin values
    static const unsigned IRSENSOR_LEFT_PIN = 0;
    static const unsigned IRSENSOR_RIGHT_PIN = 0;
    static const unsigned ULTRASONIC_PIN = 0;
    // static const unsigned MOTOR_LEFT_PIN = 17
    // ...
    // ...
    // static const unsigned RANGE_FINDER_FRONT_PIN = 18

    static const unsigned NUM_RANGE_FINDERS = 3;
    static const unsigned IRSENSOR_LEFT_MIN_DISTANCE = 20;
    static const unsigned IRSENSOR_LEFT_MAX_DISTANCE = 150;
    static const unsigned IRSENSOR_RIGHT_MIN_DISTANCE = 20;
    static const unsigned IRSENSOR_RIGHT_MAX_DISTANCE = 150;
    static const unsigned ULTRASONIC_MIN_DISTANCE = 20;
    static const unsigned ULTRASONIC_MAX_DISTANCE = 1000;

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
    unsigned speed = 100;
};