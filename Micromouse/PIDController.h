#pragma once

class PIDController {
  public:
    PIDController(float p, float i, float d, float initialError,
                  float maxIntegralError, float derivativeSmoothing = 0.0f);

    float getCorrection(float currentError, unsigned delta);
    void clear(float initialError);

  private:
    const float P;
    const float I;
    const float D;
    const float maxIntegralError;
    const float derivativeSmoothing;

    float integratedError;
    float previousError;
};
