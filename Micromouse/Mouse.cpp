#include "Mouse.h"
#include <Arduino.h>
#include <cstdio>

#if !defined(__MK66FX1M0__) && !defined(__MK20DX256__)
#include <chrono>
#include <mutex>
#include <stdexcept>
#include <thread>

std::mutex mtx;
#endif

Mouse::Mouse() : position(Maze::CELL_START) {
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

// TEMPORARY
#if !defined(__MK66FX1M0__) && !defined(__MK20DX256__)
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));

    running = true;

    while (running) {

        position = Maze::CELL_START;
        {
            std::lock_guard<std::mutex> lock(mtx);
            try {
                maze.findPath(position, Maze::CELL_FINISH);
            } catch (const std::exception &e) {
                Serial.printf("%s\n", e.what());
            }
        }
        const Path &path = maze.getPath();

        for (auto i = path.begin(); i != path.end(); ++i) {
            {
                std::lock_guard<std::mutex> lock(mtx);

                position = position + *i;
                facing = i->direction;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }

        std::lock_guard<std::mutex> lock(mtx);
        maze.generate();
    }
#endif
}

void Mouse::stopMaze() {
    std::lock_guard<std::mutex> lock(mtx);
    running = false;
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
