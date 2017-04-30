#include "IRSensor.h"
#include <Arduino.h>
#include <cmath>

float IRSensor::getDistance() const {
    // TODO: add smoothing and filtering.
    return calcDistance(readSensor());
}

bool IRSensor::calibrate() {
    Serial.printf("Distance %umm\n", MIN_RANGE);

    // TODO: Wait for button press
    delay(5000);

    upperBound = readSensor(40);

    Serial.printf("Distance %umm\n", MAX_RANGE);

    // TODO: Wait for button press
    delay(5000);

    lowerBound = readSensor(40);

    const auto samples = 3; // Only works with 3.

    unsigned x[samples];
    unsigned y[samples];

    y[0] = MIN_RANGE + 20;
    y[2] = MAX_RANGE - 30;
    y[1] = (y[0] + y[2]) / 20 * 10;

// Load fake values on PC.
#if !defined(__MK66FX1M0__) && !defined(__MK20DX256__)
    lowerBound = 373;
    upperBound = 88;
    x[0] = 248;
    x[1] = 145;
    x[2] = 105;
#else
    for (size_t s = 0; s < samples; s++) {
        Serial.printf("Distance %umm\n", y[s]);

        // TODO: Wait for button press
        delay(5000);

        x[s] = readSensor(40);

        if (x[s] <= lowerBound || x[s] >= upperBound)
            return false;
    }
#endif

    a = 0.f;
    c = 0.f;

    // Iteratively improve the fit line.
    for (size_t i = 0; i < 100; i++) {
        b = (y[2] - c) * (x[2] + a);

        c += y[0] - calcDistance(x[0]);

        b = (y[2] - c) * (x[2] + a);

        float m, q, ym;

        m = (calcDistance(x[2]) - calcDistance(x[0])) / (y[2] - y[0]);
        q = (y[2] * calcDistance(x[0]) - y[0] * calcDistance(x[2])) /
            (y[2] - y[0]);
        ym = m * y[1] + q;

        a += a + (b + c * x[1] - ym * x[1]) / (c - ym);
    }

    float r = 0.0f;
    // Calculate rough indicator of how good the calibration is.
    for (size_t s = 0; s < samples; s++) {
        r += pow(y[s] - calcDistance(x[s]), 2);
    }
    r += pow(MIN_RANGE - calcDistance(upperBound), 2);
    r += pow(MAX_RANGE - calcDistance(lowerBound), 2);

    Serial.printf("Calibration Results: \n a:%12.2f \n b:%12.2F \n c:%12.2F \n "
                  "r:%12.4F \n",
                  a, b, c, r);

    return true;
}

bool IRSensor::loadCalibration() {
    // TODO: load for real.
    a = -14.2064f;
    b = 11875.f;
    c = -10.794f;
    return true;
}

unsigned IRSensor::readSensor(unsigned samples, unsigned delay) const {
    unsigned val = 0;

    for (size_t i = 0;; i++) {
        val += analogRead(DATA_PIN);

        if (i >= samples)
            break;

        ::delay(delay);
    }

    return val / samples;
}

float IRSensor::calcDistance(unsigned sensorVal) const {
    return b / (sensorVal + a) + c;
}
