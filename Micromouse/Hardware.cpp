#include "Hardware.h"
#include "IRSensor.h"
#include "UltrasonicSensor.h"
#include <Arduino.h>
#include <algorithm>
#include <cmath>
#include <limits>

Hardware::Hardware()
    : leftMotor(MOTOR_LEFT_EN_PIN, MOTOR_LEFT_PH_PIN, ENCODER_LEFT_A_PIN,
                ENCODER_LEFT_B_PIN)
    , rightMotor(MOTOR_RIGHT_EN_PIN, MOTOR_RIGHT_PH_PIN, ENCODER_RIGHT_A_PIN,
                 ENCODER_RIGHT_B_PIN)
    , led(LED_PIN)
    , speedPID(0.002f, 0.0000002f, 100.0f, 0.0f, 30.0f, 0.6f)
    , distancePID(0.02f, 0.0f, 100.0f, 0.0f, 1.0f, 0.8f)
    , leftPID(0.0005f, 0.0f, 0.0f, 0.0f, 0.05f)
    , rightPID(0.0005f, 0.0f, 0.0f, 0.0f, 100.0f) {
    led.turnOn();
    analogWriteResolution(Hardware::WRITE_RESOLUTION);
    setSpeed(1000 /*mmps*/);
    // initRangeFinders();
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

    speedPID.clear(-countsPerSecond, -20.0f);
    distancePID.clear(0.0f);
    deltaTime();

    leftMotor.resetCounts();
    rightMotor.resetCounts();

    // Times 2 since two wheels. This prevents dividing by 2 later on.
    auto targetCounts = long(2 * mm * COUNT_PER_MM);
    long traveledCounts = 0;

    // [-1.0,1.0]
    float leftSpeedFactor;
    float rightSpeedFactor;

    while (true) {
        auto dtime = deltaTime();

        auto countsLeft = leftMotor.getCounts();
        auto countsRight = rightMotor.getCounts();

        traveledCounts += leftMotor.getCounts() + rightMotor.getCounts();

        leftMotor.resetCounts();
        rightMotor.resetCounts();

        auto distanceErr = targetCounts - traveledCounts;
        distanceErr = std::max(distanceErr, -long(COUNT_PER_NODE / 10));
        distanceErr = std::min(distanceErr, long(COUNT_PER_NODE / 10));

        auto distanceCorr =
            distancePID.getCorrection(distanceErr, dtime, false);
        distanceCorr = std::max(distanceCorr, -1.0f);
        distanceCorr = std::min(distanceCorr, 1.0f);

        leftSpeedFactor = rightSpeedFactor = distanceCorr;

        // float leftGap = rangeFinders[LEFT]->getDistance();
        // float leftGapCorr =
        //    leftPID.getCorrection(leftGap - LEFT_GAP, dtime, false);

        // if (leftGapCorr < 0) {
        //    rightSpeedFactor += leftGapCorr;
        //}

        // float rightGap = rangeFinders[RIGHT]->getDistance();
        // float rightGapCorr =
        //    rightPID.getCorrection(rightGap - RIGHT_GAP, dtime);

        // if (rightGapCorr < 0) {
        //    leftSpeedFactor += rightGapCorr;
        //}

        // `countsPerSecond`
        auto avgCPS = (countsRight + countsLeft) / (dtime * 2.0f) * 1E6;

        auto speedCorrRaw =
            speedPID.getCorrection(countsPerSecond - avgCPS, dtime, false);
        auto speedCorr = powf(1.06f, speedCorrRaw);

        //  if (targetCounts - traveledCounts < long(COUNT_PER_NODE * 2)) {

        if (avgCPS < 50.0f && std::fabs(targetCounts - traveledCounts) < 50) {
            leftMotor.off();
            rightMotor.off();
            return traveledCounts * MM_PER_COUNT;
        }
        // }
        // Serial.printf("CNT: %i, ", targetCounts - traveledCounts);
        // Serial.printf("CTL: %i, ", countsLeft);
        // Serial.printf("CTR: %i, ", countsRight);
        // Serial.printf("SPC: %f, ", speedCorr);
        // Serial.printf("DST: %f, ", distanceCorr);
        Serial.printf("LSF: %f, ", leftSpeedFactor * 3);
        Serial.printf("RSF: %f, ", rightSpeedFactor * 3);
        // Serial.printf("ASP: %f", avgCPS);
        Serial.printf("\n");

        leftMotor.setSpeed(leftSpeedFactor * speedCorr);
        rightMotor.setSpeed(rightSpeedFactor * speedCorr);

        // Loop approximately every 0.5mm traveled.
        // We will want to try a range of values here to find the best.
        delayMicroseconds(10000);
    }

    return 0;
}

void Hardware::rotate(int deg) {
    (void)deg;
    // TODO
}

// void Hardware::checkWall(Relation relation) {
//    // TODO
//}

// bool Hardware::isWall(Relation relation) {
//    // TODO
//    return false;
//}

void Hardware::setSpeed(unsigned mmps) {
    countsPerSecond = COUNT_PER_MM * mmps;
    // Serial.printf("Target Speed: %f CPS\n", countsPerSecond);
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

void Hardware::testMotorSingle() {

    // for (float i = 0.0; i < 1.0f; i += 0.001 )
    //{
    //	leftMotor.se
    //}

    Serial.printf("Testing LeftMotor\n");
    leftMotor.setSpeed(0.25f);
    delay(2000);
    leftMotor.setSpeed(0.0f);
    delay(1000);
    leftMotor.setSpeed(0.5f);
    delay(2000);
    leftMotor.setSpeed(0.0f);
    delay(1000);
    leftMotor.setSpeed(-1.0f);
    delay(2000);
    leftMotor.off();

    delay(2000);

    Serial.printf("Testing RightMotor\n");
    rightMotor.setSpeed(0.25f);
    delay(2000);
    rightMotor.setSpeed(0.0f);
    delay(1000);
    rightMotor.setSpeed(0.5f);
    delay(2000);
    rightMotor.setSpeed(0.0f);
    delay(1000);
    rightMotor.setSpeed(-1.0f);
    delay(2000);
    rightMotor.off();
}

void Hardware::testMotorPair() {
    Serial.printf("Testing Motor Pair\n");
    leftMotor.setSpeed(0.25f);
    rightMotor.setSpeed(0.25f);
    delay(2000);
    leftMotor.setSpeed(0.0f);
    rightMotor.setSpeed(0.0f);
    delay(2000);
    leftMotor.setSpeed(0.5f);
    rightMotor.setSpeed(0.5f);
    delay(2000);
    leftMotor.setSpeed(0.0f);
    rightMotor.setSpeed(0.0f);
    delay(2000);
    leftMotor.setSpeed(-1.0f);
    rightMotor.setSpeed(-1.0f);
    delay(2000);
    leftMotor.off();
    rightMotor.off();
}

void Hardware::testMovement() {
    Serial.printf("Testing Movement\n");
    moveForward(10000, false, false);
    delay(2000);
    moveForward(200, false, false);
    delay(2000);
    moveForward(300, false, false);
    delay(2000);
}

void Hardware::testRotate() {
    Serial.printf("Testing Rotate\n");

    rotate(10);
    delay(2000);
    rotate(100);
    delay(2000);
    rotate(360);
    delay(2000);
    rotate(-10);
    delay(2000);
    rotate(-180);
    delay(2000);
    rotate(-360);
    delay(2000);
}

void Hardware::testRangeFinderSingle(Relation relation, unsigned iterations,
                                     unsigned delay) {
    Serial.printf("Testing Rangefinder %u\n", relation);

    if (iterations == 0) {
        iterations = std::numeric_limits<unsigned>::max();
    }

    for (size_t i = 0; i < iterations; i++) {
        auto dist = rangeFinders[relation]->getDistance();
        Serial.printf("%f\n", dist);
        ::delay(delay);
    }
}

void Hardware::testRangeFindersAll(unsigned iterations, unsigned delay) {
    Serial.printf("Testing All Rangefinders \n");

    if (iterations == 0) {
        iterations = std::numeric_limits<unsigned>::max();
    }

    for (size_t i = 0; i < iterations; i++) {
        auto distLeft = rangeFinders[LEFT]->getDistance();
        auto distFront = rangeFinders[FRONT]->getDistance();
        auto distRight = rangeFinders[RIGHT]->getDistance();
        Serial.printf("%f, %f, %f\n", distLeft, distFront, distRight);
        ::delay(delay);
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

uint32_t Hardware::DeltaTime::operator()() {
    auto deltaTime = micros() - previousTime;
    previousTime += deltaTime;
    return deltaTime;
}

void Hardware::DeltaTime::reset() {
    previousTime = micros();
}
