#pragma once

class PIDController {
  public:
    class Delta {
      public:
        Delta(){};
        virtual unsigned operator()() const {
            return 0;
        };
    };

    PIDController(float p, float i, float d, const Delta &delta,
                  float initialError, float maxIntegralError,
                  float derivativeSmoothing = 0.0f);

    float getCorrection(float currentError);
    void clear(float initialError);

  private:
    const float P;
    const float I;
    const float D;

    const Delta &getDelta;
    float maxIntegralError;
    float derivativeSmoothing;
    float integratedError;
    float previousError;
};
