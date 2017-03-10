#define printf Serial.printf
#include "Mouse.h"

extern "C" {
    void *_sbrk() {
        return 0;
    }
}


void setup() {
    // put your setup code here, to run once:
    Mouse mouse;
}

void loop() {
    // put your main code here, to run repeatedly:
}
