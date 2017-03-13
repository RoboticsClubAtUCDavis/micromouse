#pragma once

class Motor {
  public:
    Motor(void);
    Motor(unsigned int en, unsigned int in1, unsigned int in2);
    void off(void);
    void setSpeed(float speed);
    ~Motor();

  private:
    unsigned int en, in1, in2;
};
