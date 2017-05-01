#include "Mouse.h"
#include <Arduino.h>
#include <assert.h>
#include <cstdio>

#if !defined(__MK66FX1M0__) && !defined(__MK20DX256__)
#include "../Simulation/simulate.h"
#include <chrono>
#include <mutex>
#include <stdexcept>
#include <thread>

// Not sure why Travis does not like when this is in simulate.cpp
float SIMULATION_SPEED = 1.0f;
std::mutex mtx;
#endif

Mouse::Mouse() : position(Maze::CELL_START) {
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
            mapMazeStrategy3();
            break;
        default:
            break;
    }
}

void Mouse::runMaze() {
// TODO

// TEMPORARY
#if !defined(__MK66FX1M0__) && !defined(__MK20DX256__)

    // Give time for the window to open
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    unsigned cycle = 0;

    running = true;
    while (running) {
        if (++cycle % 100 == 0)
            printf("Cycle %u\n", cycle);

        try {
            mapMaze();
        } catch (const std::exception &e) {
            Serial.printf("%s\n", e.what());
        }

        std::lock_guard<std::mutex> lock(mtx);
        maze.reset();
        virtualMaze.generate();
        position = Maze::CELL_START;
        facing = N;
    }
#endif
}

#if !defined(__MK66FX1M0__) && !defined(__MK20DX256__)
void Mouse::stopMaze() {
    std::lock_guard<std::mutex> lock(mtx);
    running = false;
}
#endif

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

void Mouse::followPath(const Path &path, bool useCaution) {
// TODO

// TEMPORARY
// This is just used to test mapping.
// Many of the actions here need to be delegated to the proper helper
// functions and to `Hardware`.
#if !defined(__MK66FX1M0__) && !defined(__MK20DX256__)
    std::unique_lock<std::mutex> lock(mtx);
    if (useCaution) {
        bool pathBroken = false;

        for (auto &i : path) {
            NodeCoordinate nextPos = position + i.direction;

            Direction direction = Direction((int(facing) + int(W)) % NONE);

            for (int i = 0; i < 3; i++) {
                maze.setExplored(position + direction);

                if (virtualMaze.isWall(position + direction)) {
                    maze.setWall(position + direction);
                    if (position + direction == nextPos) {
                        pathBroken = true;
                    }
                } else {
                    if (maze.withinBounds(position +
                                          DirectionVector(direction, 2))) {
                        maze.setExplored(position +
                                         DirectionVector(direction, 2));
                    }
                }

                if (direction == facing) {
                    for (size_t mag = 1; mag <= 8; mag++) {
                        maze.setExplored(position +
                                         DirectionVector(direction, mag));

                        if (virtualMaze.isWall(
                                position + DirectionVector(direction, mag))) {
                            maze.setWall(position +
                                         DirectionVector(direction, mag));
                            break;
                        }
                    }
                }

                direction = Direction((int(direction) + 2) % NONE);
            }

            if (pathBroken) {
                return;
            }

            lock.unlock();
            move(i, false, useCaution);
            lock.lock();
        }
    } else {
        for (auto &i : path) {
            lock.unlock();
            move(i, false, useCaution);
            lock.lock();
        }
    }

#else
    for (auto &i : path) {
        // TODO: have `keepGoing` be conditional based on next movement
        move(i, false, useCaution);
        // TODO
    }
#endif
}

void Mouse::followPath(bool useCaution) {
    followPath(maze.getPath(), useCaution);
}

unsigned Mouse::move(DirectionVector movement, bool keepGoing,
                     bool useCaution) {
#if !defined(__MK66FX1M0__) && !defined(__MK20DX256__)
    (void)keepGoing;
    (void)useCaution;

    std::unique_lock<std::mutex> lock(mtx);

    lock.unlock();
    if (SIMULATION_SPEED < 10.0f) {
        std::this_thread::sleep_for(
            std::chrono::milliseconds(long(300 / SIMULATION_SPEED)));
    }
    lock.lock();
#else
    // TODO: Need better error handling with sensors to keep in correct
    // position;
    // It could be in Hardware, but probably here since Hardware doesn't know
    // about cells
    //
    // XXX diagonals will need to be handled, if we implement them
    bot.rotate(DirOp::angleDiff(facing, movement.direction));
    // XXX if diagonals are implemented, the distance also needs adjustment
    bot.moveForward(movement.magnitude * 90, keepGoing, useCaution);
#endif

    position = position + movement;
    facing = movement.direction;

    return 0;
}

void Mouse::mapMazeExhaustive() {
    // TODO
}

NodeCoordinate Mouse::findOtherNode(NodeCoordinateList &coordList,
                                    NodeCoordinate pos) {
    // List must be even size since we assume they are pairs.
    assert(coordList.size() % 2 == 0);

    for (int i = coordList.size() - 1; i >= 0; i--) {
        if (coordList[i] == pos) {
            if (i % 2 == 0)
                return coordList[i + 1];
            else
                return coordList[i - 1];
        }
    }

    // The given `NodeCoordinate` was not found.
    assert(false);
}

void Mouse::mapMazeStrategy3() {
    NodeCoordinateList coordList;
    bool foundFinish = false;

    {
#if !defined(__MK66FX1M0__) && !defined(__MK20DX256__)
        std::lock_guard<std::mutex> lock(mtx);
#endif
        // Set the start and finish nodes as explored.
        maze.setExplored(Maze::NODE_START);
        maze.setExplored(Maze::NODE_FINISH);
    }

    while (true) {
        // Find a path from start to finish using explored and unexplored
        // nodes.
        maze.findPath(Maze::NODE_START, Maze::NODE_FINISH, false, facing);

        // Make a list of all coordinates where the path transitions from
        // explored to unxplored or unexplored to explored nodes.
        // They will always come in pairs.
        maze.findNodeCoordPairs(coordList);

        // If the shortest path was completely contained within explored
        // nodes then the list is empty and we can stop mapping.
        if (coordList.size() == 0)
            break;

        // Travel to the nearest node we just found by only pathfinding on
        // explored nodes to guarantee we can get there safely.
        maze.findPath(position, coordList, true, facing);
        followPath(false);

        NodeCoordinate subGoal = findOtherNode(coordList, position);

        do {
            // Attempt to travel from the first node of the pair to the
            // second. Stop if the path is blocked.
            maze.findPath(position, subGoal, false, facing);
            followPath(true);

            // Find a new path from start to finish incase it was changed by
            // the recently discovered walls.
            maze.findPath(Maze::NODE_START, Maze::NODE_FINISH, false, facing);

            // If the shortest path was completely contained within explored
            // nodes then the list is empty and we can stop mapping.
            if (coordList.size() == 0)
                goto FINISH_MAPPING;

        } while (position != subGoal);

        // The first time this line is reached is when we are at the finish
        // of the maze. We can then remove all remaining nodes around the
        // center because we know they must be walls.
        if (!foundFinish) {
            maze.closeExcessFinishNodes();
            foundFinish = true;
        }

        coordList.clear();
    }

// exit point for the inner loop
FINISH_MAPPING:
    return;
}
