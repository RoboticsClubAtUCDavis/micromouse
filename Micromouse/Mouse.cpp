#include "Mouse.h"
#include <Arduino.h>
#include <chrono>
#include <cstdio>
#include <mutex>
#include <stdexcept>
#include <thread>
using namespace std::literals::chrono_literals;

std::mutex mtx;

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

    std::this_thread::sleep_for(3000ms);
    int x = 0;
    while (true) {

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
            std::this_thread::sleep_for(100ms);
        }

        std::lock_guard<std::mutex> lock(mtx);
        maze.generate();
    }
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
