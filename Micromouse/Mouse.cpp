#include "Mouse.h"
#include <Arduino.h>
#include <cstdio>

Mouse::Mouse() {
    pinMode(13, OUTPUT);
    digitalWrite(13, HIGH);
    Serial.printf("Mouse Created!");
}

void Mouse::mapMaze() {
    switch (mappingStrategy) {
        case Mouse::EXHAUSTIVE:
            mapMazeExhaustive();
            break;
        case Mouse::STRAT2:
            // TODO
            break;
        case Mouse::STRAT3:
            // TODO
            break;
        default:
            break;
    }
}

void Mouse::runMaze() {
    // TODO
}

void Mouse::setMappingSpeed(unsigned mmps) {
    mappingSpeed = mmps;
}

void Mouse::setRunningSpeed(unsigned mmps) {
    runningSpeed = mmps;
}

void Mouse::setMappingStrategy(MappingStrategy strategy) {
    mappingStrategy = strategy;
}

void Mouse::reset() {
    // TODO
}

Maze &Mouse::getMaze() {
    return maze;
}

void Mouse::moveTo(CellCoordinate position) {
    (void)position;
    // TODO
}

void Mouse::followPath(const Path &path) {
    (void)path;
    // TODO
}

unsigned Mouse::move(DirectionVector movement, bool keepGoing,
                     bool useCaution) {
    (void)movement;
    (void)keepGoing;
    (void)useCaution;
    // TODO
    return 0;
}

void Mouse::mapMazeExhaustive() {
    // TODO
}
