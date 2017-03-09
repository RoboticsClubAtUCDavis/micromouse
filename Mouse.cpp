#include "Mouse.h"

Mouse::Mouse() {
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
    // TODO
}

void Mouse::setRunningSpeed(unsigned mmps) {
    // TODO
}

void Mouse::setMappingStrategy(MappingStrategy strat) {
    // TODO
}

void Mouse::reset() {
    // TODO
}

void Mouse::moveTo(CellCoordinate position) {
    // TODO
}

void Mouse::followPath(const Path &path) {
    // TODO
}

unsigned Mouse::move(DirectionVector movement, bool keepGoing,
                     bool useCaution) {
    // TODO
    return 0;
}

void Mouse::mapMazeExhaustive() {
    // TODO
}
