#pragma once

// TODO: Try to get this library to compile with the spoofed Teensy, and remove
// ifdefs
#if defined(__MK66FX1M0__) || defined(__MK20DX256__)
#include <Encoder.h>
#endif

class Motor {
  public:
    Motor(void);
    Motor(unsigned en, unsigned in1, unsigned in2, unsigned encA,
          unsigned encB);
    ~Motor();

    void off(void);
    void setSpeed(float speed);
    long getCounts(void);
    void resetCounts(void);
    float getSecondsPerCount(void);

  private:
    unsigned en, in1, in2;

    float secondsPerCount = 0.0f;

    long lastCounts = 0;
    long lastMicros = 0;

#if defined(__MK66FX1M0__) || defined(__MK20DX256__)
    Encoder encoder;
#endif
};
