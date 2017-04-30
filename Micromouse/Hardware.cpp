#include "Hardware.h"
#include "IRSensor.h"
#include "UltrasonicSensor.h"
#include <cmath>

const float Hardware::COUNT_PER_MM = 7.23957f;
const float Hardware::MM_PER_COUNT = 1.0f / Hardware::COUNT_PER_MM;
const unsigned Hardware::MM_PER_NODE = 90;
const unsigned Hardware::COUNT_PER_NODE =
    Hardware::COUNT_PER_MM * Hardware::MM_PER_NODE;

Hardware::Hardware()
    : led(Hardware::LED_PIN)
    , speedPID(1.0f, 1.0f, 1.0f, 0.0f, 100.0f)
    , distancePID(1.0f, 1.0f, 1.0f, 0.0f, 100.0f)
    , leftPID(1.0f, 1.0f, 1.0f, 0.0f, 100.0f)
    , rightPID(1.0f, 1.0f, 1.0f, 0.0f, 100.0f) {
    led.turnOn();
    setSpeed(100 /*mmps*/);
    initRangeFinders();
    // TODO init rest of components.
}

Hardware::~Hardware() {
    for (auto i : rangeFinders) {
        delete i;
    }
}

unsigned Hardware::moveForward(unsigned mm, bool keepGoing, bool useCaution) {
    // TODO: `keepGoing` and `useCaution` are ignored. We should test that this
    // works before adding more cases.
    (void)keepGoing;
    (void)useCaution;

    // Times 2 since two wheels. This prevents dividing by 2 later on.
    auto targetCounts = unsigned(2 * mm * COUNT_PER_MM);
    unsigned traveledCounts = 0;

    // [-1.0,1.0]
    float leftSpeedFactor = 0.0f;
    float rightSpeedFactor = 0.0f;

    while (true) {
        unsigned dtime = deltaTime();

        traveledCounts += leftMotor.getCounts() + rightMotor.getCounts();

        leftMotor.resetCounts();
        rightMotor.resetCounts();

        // `secondsPerCount` is doubled to account for not dividing by two here.
        float avgSPC =
            leftMotor.getSecondsPerCount() + rightMotor.getSecondsPerCount();

        float speedCorr =
            speedPID.getCorrection(secondsPerCount - avgSPC, dtime);

        leftSpeedFactor += speedCorr;
        rightSpeedFactor += speedCorr;

        float leftGap = rangeFinders[LEFT]->getDistance();
        float leftGapCorr = leftPID.getCorrection(LEFT_GAP - leftGap, dtime);

        if (leftGapCorr > 0) {
            leftSpeedFactor -= leftGapCorr;
        } else {
            rightSpeedFactor += leftGapCorr;
        }

        float rightGap = rangeFinders[RIGHT]->getDistance();
        float rightGapCorr =
            rightPID.getCorrection(RIGHT_GAP - rightGap, dtime);

        if (rightGapCorr > 0) {
            rightSpeedFactor -= rightGapCorr;
        } else {
            leftSpeedFactor += rightGapCorr;
        }

        if (targetCounts - traveledCounts < COUNT_PER_NODE * 2) {
            float distanceCorr =
                distancePID.getCorrection(targetCounts - traveledCounts, dtime);

            if (avgSPC < 0.001f && std::fabs(distanceCorr) < 0.1f) {
                leftMotor.off();
                rightMotor.off();
                return traveledCounts * MM_PER_COUNT;
            }
        }

        leftMotor.setSpeed(leftSpeedFactor);
        rightMotor.setSpeed(rightSpeedFactor);

        // Loop approximately every 0.5mm traveled.
        // We will want to try a range of values here to find the best.
        delayMicroseconds(avgSPC * 1E6f / 2.0f * COUNT_PER_MM / 2.0f);
    }

    return 0;
}

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
    // Times 2 since two wheels. This prevents dividing by 2 later on.
    secondsPerCount = MM_PER_COUNT / mmps * 2;
}

// void Hardware::calibrateMotors() {
//    // TODO
//}

void Hardware::calibrateRangeFinders() {
    for (auto i : rangeFinders) {
        if (i->calibrate()) {
            i->saveCalibration();
        } else {
            Serial.printf("Error during calibration\n");
            break;
        }
    }
}

void Hardware::calibrateRangeFinder(Relation relation) {
    if (relation != NO_RELATION) {
        if (rangeFinders[relation]->calibrate()) {
            rangeFinders[relation]->saveCalibration();
        } else {
            Serial.printf("Error during calibration\n");
        }
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
