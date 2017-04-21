#include "Hardware.h"
#include "IRSensor.h"
#include "UltrasonicSensor.h"

Hardware::Hardware()
    : speedPID(1.0f, 1.0f, 1.0f, 0.0f, 100.0f)
    , distancePID(1.0f, 1.0f, 1.0f, 0.0f, 100.0f)
    , leftPID(1.0f, 1.0f, 1.0f, 0.0f, 100.0f)
    , rightPID(1.0f, 1.0f, 1.0f, 0.0f, 100.0f) {

    initRangeFinders();
    // TODO init rest of components.
}

Hardware::~Hardware() {
    for (auto i : rangeFinders) {
        delete i;
    }
}

// unsigned Hardware::moveForward(unsigned mm, bool keepGoing, bool useCaution)
// {
//    // TODO
//    return 0;
//}

// void Hardware::rotate(int deg) {
//    // TODO
//}

// void Hardware::checkWall(Relation relation) {
//    // TODO
//}

// bool Hardware::isWall(Relation relation) {
//    // TODO
//    return false;
//}

void Hardware::setSpeed(unsigned mmps) {
    speed = mmps;
}

// void Hardware::calibrateMotors() {
//    // TODO
//}

void Hardware::calibrateRangeFinders() {
    for (auto i : rangeFinders) {
        i->calibrate();
    }
}

void Hardware::calibrateRangeFinder(Relation relation) {
    if (relation != NO_RELATION) {
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

    for (auto i : rangeFinders) {
        i->loadCalibration();
    }
}

unsigned Hardware::DeltaTime::operator()() {
    auto deltaTime = micros() - previousTime;
    previousTime += deltaTime;
    return deltaTime;
}

void Hardware::DeltaTime::reset() {
    previousTime = micros();
}
