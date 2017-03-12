
// Define missing definitions
namespace std {
void __throw_bad_alloc() {
    Serial.printf("Unable to allocate memory\n");
    exit(1);
}

void __throw_length_error(char const *e) {
    Serial.printf("Length Error : %s\n", e);
    exit(1);
}
}

extern "C" {
int _getpid() {
    return -1;
}
int _kill(int pid, int sig) {
    return -1;
}
int _write() {
    return -1;
}
}

#include "Mouse.h"

void setup() {
    Serial.begin(9600);
    delay(2000);
    Mouse t;
}

void loop() {
}
