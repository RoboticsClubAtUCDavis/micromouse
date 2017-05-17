#include <Arduino.h>
#undef min
#undef max

#include "Mouse.h"
#include <assert.h>
#include <cstdio>
#include <stack>

#if !defined(__MK66FX1M0__) && !defined(__MK20DX256__)
#include "../Simulation/simulate.h"
#include <algorithm>
#include <chrono>
#include <mutex>
#include <stdexcept>
#include <thread>

// Not sure why Travis does not like when this is in simulate.cpp
float SIMULATION_SPEED = 1.0f;
std::mutex mtx;
#endif

const Mouse::MappingStrategy Mouse::MAP_STRATS[Mouse::NO_STRAT] = {
    Mouse::STRAT3, Mouse::BFS, Mouse::DFS};

Mouse::Mouse() : position(Maze::CELL_START) {
    Serial.printf("Mouse Created!\n");
}

void Mouse::testMode(TestMode mode) {
    switch (mode) {
        case TestMode::TEST_MOTOR_SINGLE:
            bot.testMotorSingle();
            break;
        case TestMode::TEST_MOTOR_PAIR:
            bot.testMotorPair();
            break;
        case TestMode::TEST_MOVEMENT:
            bot.testMovement();
            break;
        case TestMode::TEST_ROTATE:
            bot.testRotate();
            break;
        case TestMode::TEST_RANGE_FINDER_SINGLE:
            bot.testRangeFinderSingle(LEFT);
            bot.testRangeFinderSingle(RIGHT);
            break;
        case TestMode::TEST_RANGE_FINDER_ALL:
            bot.testRangeFindersAll(0,10);
            break;
        default:
            break;
    }
}

Mouse::MappingScore Mouse::mapMaze() {
    totalMovementCost = 0;
    totalMovements = 0;

    switch (mappingStrategy) {
        case Mouse::STRAT3:
            mapMazeStrategy3();
            break;
        case Mouse::BFS:
            mapMazeBFS();
            break;
        case Mouse::DFS:
            mapMazeDFS();
            break;
        default:
            break;
    }

    maze.findPath(position, Maze::NODE_START, true, facing);
    followPath(false);

    Serial.printf("Mapping Complete - Movements: %4u, TotalCost: %6u \n\n",
                  totalMovements, totalMovementCost);

    return MappingScore{totalMovements, totalMovementCost};
}

void Mouse::runMaze() {
// TODO

// TEMPORARY
#if !defined(__MK66FX1M0__) && !defined(__MK20DX256__)

    // Give time for the window to open
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    unsigned cycle = 0;
    auto seed = uint16_t(time(NULL));

    running = true;
    while (running) {
        if (++cycle % 100 == 0)
            printf("Cycle %u\n", cycle);

        try {
            mapMaze();
        } catch (const std::exception &e) {
            Serial.printf("%s\n", e.what());
        }

        Maze newMaze = Maze::generate(seed + cycle);
        std::lock_guard<std::mutex> lock(mtx);
        virtualMaze = newMaze;
        maze.reset();
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

    if (SIMULATION_SPEED < 1000.0f) {
        std::this_thread::sleep_for(
            std::chrono::milliseconds(long(300 / SIMULATION_SPEED)));
    }

    std::unique_lock<std::mutex> lock(mtx);

    for (auto relation : RELATIONS) {
        maze.setExplored(position + DirOp::relToDir(relation, facing));
    }
#else
    // TODO: Need better error handling with sensors to keep in correct
    // position;
    // It could be in Hardware, but probably here since Hardware doesn't
    // know about cells
    //
    // XXX diagonals will need to be handled, if we implement them
    bot.rotate(DirOp::angleDiff(facing, movement.direction));
    // XXX if diagonals are implemented, the distance also needs adjustment
    bot.moveForward(movement.magnitude * 90, keepGoing, useCaution);
#endif

    totalMovementCost += Maze::calculateMovementCost(facing, movement.direction,
                                                     movement.magnitude);
    totalMovements++;

    position = position + movement;
    facing = movement.direction;

#if !defined(__MK66FX1M0__) && !defined(__MK20DX256__)

    for (auto relation : RELATIONS) {
        if (virtualMaze.isWall(position + DirOp::relToDir(relation, facing)))
            maze.setWall(position + DirOp::relToDir(relation, facing));
    }

    maze.setExplored(position);
    maze.getNode(position).visited = true;
#endif

    return 0;
}

unsigned Mouse::move(Relation relation, bool keepGoing, bool useCaution) {
    return move(DirectionVector(DirOp::relToDir(relation, facing), 1),
                keepGoing, useCaution);
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

void Mouse::mapMazeBFS() {
    NodeCoordinateList coordList;

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

        // Attempt to travel to the finish.
        // Stop if the path is blocked.
        maze.findPath(position, Maze::NODE_FINISH, false, facing);
        followPath(true);

        if (position == Maze::NODE_FINISH) {
            maze.closeExcessFinishNodes();
        }

        coordList.clear();
    }
}

void Mouse::mapMazeDFS() {
    NodeCoordinateList coordList;
    std::stack<NodeCoordinate> intersections;

    {
#if !defined(__MK66FX1M0__) && !defined(__MK20DX256__)
        std::lock_guard<std::mutex> lock(mtx);
#endif
        // Set the start and finish nodes as explored.
        maze.setExplored(Maze::NODE_START);
        maze.setExplored(Maze::NODE_FINISH);

// TODO replace with hardware function that uses sensors
#if !defined(__MK66FX1M0__) && !defined(__MK20DX256__)
        for (auto relation : RELATIONS) {
            if (virtualMaze.isWall(position, DirOp::relToDir(relation, facing)))
                maze.setWall(position, DirOp::relToDir(relation, facing));
        }
#endif

        maze.setExplored(position);
        maze.getNode(position).visited = true;
    }

    do {
        coordList.clear();

        bool isWall[NO_RELATION];
        isWall[LEFT] = maze.isWall(LEFT, position, facing);
        isWall[FRONT] = maze.isWall(FRONT, position, facing);
        isWall[RIGHT] = maze.isWall(RIGHT, position, facing);

        bool isExplored[NO_RELATION];
        isExplored[LEFT] = maze.isExplored(LEFT, position, facing);
        isExplored[FRONT] = maze.isExplored(FRONT, position, facing);
        isExplored[RIGHT] = maze.isExplored(RIGHT, position, facing);

        // Favor going straight.
        if (!isWall[FRONT] && !isExplored[FRONT]) {
            if (!isWall[LEFT] && !isExplored[LEFT]) {
                intersections.push(position + DirOp::relToDir(LEFT, facing));
            }

            if (!isWall[RIGHT] && !isExplored[RIGHT]) {
                intersections.push(position + DirOp::relToDir(RIGHT, facing));
            }

            move(FRONT, false, false);
        } else if (!isWall[RIGHT] && !isExplored[RIGHT]) {
            if (!isWall[LEFT] && !isExplored[LEFT]) {
                intersections.push(position + DirOp::relToDir(LEFT, facing));
            }

            move(RIGHT, false, false);
        } else if (!isWall[LEFT] && !isExplored[LEFT]) {
            move(LEFT, false, false);
        }
        // All three directions have a wall or have been explored.
        // Backtrack to last intersection.
        else {
            NodeCoordinate dest;

            do {
                dest = intersections.top();
                intersections.pop();
            } while (maze.getNode(dest).visited);

            maze.findPath(position, dest, true, facing);
            followPath(false);
        }
        // Find a path from start to finish using explored and unexplored
        // nodes.
        maze.findPath(Maze::NODE_START, Maze::NODE_FINISH, false, facing);

        // Make a list of all coordinates where the path transitions from
        // explored to unxplored or unexplored to explored nodes.
        maze.findNodeCoordPairs(coordList);

        // If the shortest path was completely contained within explored
        // nodes then the list is empty and we can stop mapping.
    } while (coordList.size() > 0);
}

#if !defined(__MK66FX1M0__) && !defined(__MK20DX256__)
void Mouse::rankMappingStrategies(const unsigned cycles) {
    // Give time for the window to open
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    auto seed = uint16_t(time(NULL));

    struct MappingScoreStats {
        MappingScore min{std::numeric_limits<unsigned>::max(),
                         std::numeric_limits<unsigned>::max()};
        MappingScore max{0, 0};
        MappingScore avg{0, 0};
    };

    std::vector<MappingScoreStats> stats{NO_STRAT, MappingScoreStats{}};

    Maze newMaze = virtualMaze;

    running = true;

    for (size_t i = 0; i < cycles && running; i++) {
        for (auto strategy : MAP_STRATS) {

            mappingStrategy = strategy;
            MappingScore results;

            try {
                results = mapMaze();
            } catch (const std::exception &e) {
                Serial.printf("%s\n", e.what());
            }

            stats[strategy].avg.moves += results.moves;
            stats[strategy].avg.cost += results.cost;

            stats[strategy].min.cost =
                std::min(stats[strategy].min.cost, results.cost);
            stats[strategy].min.moves =
                std::min(stats[strategy].min.moves, results.moves);

            stats[strategy].max.cost =
                std::max(stats[strategy].max.cost, results.cost);
            stats[strategy].max.moves =
                std::max(stats[strategy].max.moves, results.moves);

            std::lock_guard<std::mutex> lock(mtx);
            maze.reset();
            virtualMaze = newMaze;
            position = Maze::CELL_START;
            facing = N;
        }

        newMaze = Maze::generate(seed + i);
    }

    for (auto strategy : MAP_STRATS) {
        Serial.printf(
            "S:%i, Moves:(%4u, %4u, %4u) Costs:(%6u, %6u, %6u) \n", strategy,
            stats[strategy].min.moves, stats[strategy].max.moves,
            stats[strategy].avg.moves / cycles, stats[strategy].min.cost,
            stats[strategy].max.cost, stats[strategy].avg.cost / cycles);
    }
}
#endif
