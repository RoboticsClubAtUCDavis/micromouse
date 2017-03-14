#include "Hardware.h"
#include "IRSensor.h"
#include "UltrasonicSensor.h"
#include <cmath>

const float Hardware::COUNT_PER_MM = 7.23957f;
const float Hardware::MM_PER_COUNT = 1.0f / Hardware::COUNT_PER_MM;
const unsigned Hardware::MM_PER_NODE = 90;
const unsigned Hardware::COUNT_PER_NODE = Hardware::COUNT_PER_MM * Hardware::MM_PER_NODE;

Hardware::Hardware()
    : speedPID(1.0f, 1.0f, 1.0f, 0.0f, 100.0f)
    , distancePID(1.0f, 1.0f, 1.0f, 0.0f, 100.0f)
    , leftPID(1.0f, 1.0f, 1.0f, 0.0f, 100.0f)
    , rightPID(1.0f, 1.0f, 1.0f, 0.0f, 100.0f) {

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
    auto targetCounts = unsigned(2 * mm * COUNT_PER_MM);
    unsigned traveledCounts =
        leftMotor.getCounts() + rightMotor.getCounts();

    // [-1.0,1.0]
    float leftSpeedFactor = 0.0f;
    float rightSpeedFactor = 0.0f;

    // SPC := seconds per count

    while (true) {
        float avgSPC = (leftMotor.getSPC() + rightMotor.getSPC()) / 2.0f;
        float speedCorr =
            speedPID.getCorrection(secondsPerCount - avgSPC);

        leftSpeedFactor += speedCorr;
        rightSpeedFactor += speedCorr;

        float leftGap = rangeFinders[LEFT]->getDistance();
        float leftGapCorr = leftPID.getCorrection(LEFT_GAP - leftGap);
        if (leftGapCorr > 0)
        {
            leftSpeedFactor -= leftGapCorr;
        }
        else
        {
            rightSpeedFactor += leftGapCorr;
        }

        float rightGap = rangeFinders[RIGHT]->getDistance();
        float rightGapCorr = rightPID.getCorrection(RIGHT_GAP - rightGap);
        if (rightGapCorr > 0)
        {
            rightSpeedFactor -= rightGapCorr;
        }
        else
        {
            leftSpeedFactor += rightGapCorr;
        }

        if (targetCounts - traveledCounts < COUNT_PER_NODE * 2 )
        {
            float distanceCorr = distancePID.getCorrection(targetCounts - traveledCounts);

            if (avgSPC > 0.001f && std::fabs( distanceCorr ) < 0.1f )
            {
                leftMotor.off();
                rightMotor.off();
                return traveledCounts * MM_PER_COUNT;
            }
        }

        leftMotor.setSpeed(leftSpeedFactor);
        rightMotor.setSpeed(rightSpeedFactor);
    }

    // TODO
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
    secondsPerCount = MM_PER_COUNT / mmps;
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
