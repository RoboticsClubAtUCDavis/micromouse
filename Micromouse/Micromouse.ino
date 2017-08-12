
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
} // namespace std

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
    Serial.begin(115200);
    analogWriteFrequency(2, 29296.875);
    analogWriteFrequency(3, 29296.875);
    analogWriteResolution(11);
    delay(4000);
    Mouse mouse;
    mouse.testMode(Mouse::TestMode::TEST_MOTOR_SINGLE);
    // mouse.testMode(Mouse::TestMode::TEST_MOTOR_PAIR);
    // mouse.testMode(Mouse::TestMode::TEST_MOVEMENT);
    // mouse.testMode(Mouse::TestMode::TEST_ROTATE);
    // mouse.testMode(Mouse::TestMode::TEST_RANGE_FINDER_SINGLE);
    // mouse.testMode(Mouse::TestMode::TEST_RANGE_FINDER_ALL);
}

void loop() {
}
