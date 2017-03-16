#pragma once

// TODO: Try to get this library to compile with the spoofed Teensy, and remove
// ifdefs
#if defined(__MK66FX1M0__) || defined(__MK20DX256__)
#include <Encoder.h>
#endif

class Motor {
  public:
    Motor(void);
    Motor(unsigned en, unsigned in1, unsigned in2, unsigned enc1,
          unsigned enc2);
    void off(void);
    void setSpeed(float speed);
    long getCounts(void);
    void resetCounts(void);
    ~Motor();

  private:
    unsigned en, in1, in2;
#if defined(__MK66FX1M0__) || defined(__MK20DX256__)
    Encoder encoder;
#endif
};
