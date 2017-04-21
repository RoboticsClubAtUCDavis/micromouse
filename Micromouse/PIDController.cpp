#include "PIDController.h"
#include <algorithm>
#include <assert.h>

PIDController::PIDController(float p, float i, float d, float initialError,
                             float maxIntegralError, float derivativeSmoothing)
    : P(p)
    , I(i)
    , D(d)
    , maxIntegralError(maxIntegralError)
    , derivativeSmoothing(derivativeSmoothing)
    , integratedError(0.0f)
    , previousError(initialError) {

    assert(derivativeSmoothing >= 0.0f && derivativeSmoothing < 1.0f);
    assert(P >= 0.0f);
    assert(I >= 0.0f);
    assert(D >= 0.0f);
    assert(maxIntegralError > 0.0f);
}

float PIDController::getCorrection(float currentError, unsigned delta) {

    integratedError += currentError * delta;

    // Bound the integral error between the +/- maximums.
    integratedError = std::max(integratedError, -maxIntegralError);
    integratedError = std::min(integratedError, maxIntegralError);

    float pTerm = P * currentError;
    float iTerm = I * integratedError;

    // Not sure if this is the best way to handle `delta = 0` cases.
    float dTerm = D * ((currentError - previousError) / (delta + 1));

    previousError = derivativeSmoothing * previousError +
                    (1 - derivativeSmoothing) * currentError;

    return pTerm + iTerm + dTerm;
}

void PIDController::clear(float initialError) {
    integratedError = 0.0f;
    previousError = initialError;
}
