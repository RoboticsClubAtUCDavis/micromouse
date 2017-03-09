#pragma once
#include "Direction.h"
#include "Led.h"
#include "Motor.h"
#include "RangeFinder.h"

enum Relation { LEFT, FRONT, RIGHT };

class Hardware {
  public:
    // PIN DEFINITIONS
    // static const unsigned MOTOR_LEFT_PIN = 17
    // ...
    // ...
    // static const unsigned RANGE_FINDER_FRONT_PIN = 18

    static const unsigned NUM_RANGE_FINDERS = 3;

    Hardware();

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

  private:
    RangeFinder rangeFinders[NUM_RANGE_FINDERS];
    Motor leftMotor;
    Motor rightMotor;
    Led led;
};