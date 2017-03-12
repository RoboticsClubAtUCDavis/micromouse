#include "Hardware.h"
#include "IRSensor.h"
#include "UltrasonicSensor.h"

Hardware::Hardware() {
    initRangeFinders();
    // TODO init rest of components.
}

Hardware::~Hardware() {
    for (Relation r = LEFT; r != NO_RELATION;
         r = static_cast<Relation>(r + 1)) {
        delete rangeFinders[r];
    }
}

unsigned Hardware::moveForward(unsigned mm, bool keepGoing, bool useCaution) {
    // TODO
    return 0;
}

void Hardware::rotate(int deg) {
    // TODO
}

void Hardware::checkWall(Relation relation) {
    // TODO
}

bool Hardware::isWall(Relation relation) {
    // TODO
    return false;
}

void Hardware::setSpeed(unsigned mmps) {
    speed = mmps;
}

void Hardware::calibrateMotors() {
    // TODO
}

void Hardware::calibrateRangeFinders() {
    for (Relation r = LEFT; r != NO_RELATION;
         r = static_cast<Relation>(r + 1)) {
        rangeFinders[r]->calibrate();
    }
}

void Hardware::calibrateRangeFinder(Relation relation) {
    if (relation != NONE) {
        rangeFinders[relation]->calibrate();
    }
}

void Hardware::initRangeFinders() {
    rangeFinders[LEFT] =
        new IRSensor(IRSENSOR_LEFT_PIN, IRSENSOR_LEFT_MIN_DISTANCE,
                     IRSENSOR_LEFT_MAX_DISTANCE);

    rangeFinders[FRONT] = new UltrasonicSensor(
        ULTRASONIC_PIN, ULTRASONIC_MIN_DISTANCE, ULTRASONIC_MAX_DISTANCE);

    rangeFinders[RIGHT] =
        new IRSensor(IRSENSOR_RIGHT_PIN, IRSENSOR_RIGHT_MIN_DISTANCE,
                     IRSENSOR_RIGHT_MAX_DISTANCE);

    for (Relation r = LEFT; r != NO_RELATION;
         r = static_cast<Relation>(r + 1)) {
        rangeFinders[r]->loadCalibration();
    }
}
