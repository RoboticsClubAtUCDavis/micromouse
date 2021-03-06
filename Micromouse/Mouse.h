#pragma once
#include "Coordinate.h"
#include "Direction.h"
#include "Hardware.h"
#include "Maze.h"
#include "Path.h"

#if !defined(__MK66FX1M0__) && !defined(__MK20DX256__)
class Simulator;
class MouseDrawable;
#endif

class Mouse {
  public:
    // update array in Mouse.cpp too!
    enum MappingStrategy { STRAT3, BFS, DFS, NO_STRAT };
    static const MappingStrategy MAP_STRATS[NO_STRAT];

    enum class TestMode {
        TEST_MOTOR_SINGLE,
        TEST_MOTOR_PAIR,
        TEST_MOVEMENT,
        TEST_ROTATE,
        TEST_RANGE_FINDER_SINGLE,
        TEST_RANGE_FINDER_ALL
    };

#if !defined(__MK66FX1M0__) && !defined(__MK20DX256__)
    friend class Simulator;
    friend class MouseDrawable;

    void stopMaze();
#endif

    Mouse();

    struct MappingScore {
        unsigned moves;
        unsigned cost;
    };

    MappingScore mapMaze();

    void runMaze();

    void testMode(TestMode mode);

    // In millimeter per second.
    void setMappingSpeed(unsigned mmps);
    void setRunningSpeed(unsigned mmps);

    void setMappingStrategy(MappingStrategy strategy);

    void reset();

  private:
    void moveTo(CellCoordinate position);
    void followPath(const Path &path, bool useCaution);
    void followPath(bool useCaution);

    // If `keepGoing` is true the mouse will not stop at the end of the
    // movement. If `useCaution` is true it will stop when a wall is
    // encountered. Returns the number of nodes it was unable to move.
    unsigned move(DirectionVector movement, bool keepGoing, bool useCaution);
    unsigned move(Relation relation, bool keepGoing, bool useCaution);

    void mapMazeStrategy3();
    void mapMazeBFS();
    void mapMazeDFS();

#if !defined(__MK66FX1M0__) && !defined(__MK20DX256__)
    void rankMappingStrategies(const unsigned cycles);
#endif

    // Given one of the `NodeCoordinate`s in the `coordList` returns the
    // `NodeCoordinate` that it is paired with.
    NodeCoordinate findOtherNode(NodeCoordinateList &coordList,
                                 NodeCoordinate pos);

    Maze maze;
#if !defined(__MK66FX1M0__) && !defined(__MK20DX256__)
    Maze virtualMaze;
#endif
    Hardware bot;

    NodeCoordinate position;
    Direction facing;

    // In millimeter per second.
    int mappingSpeed = 100;
    int runningSpeed = 100;

    MappingStrategy mappingStrategy = DFS;

    bool running;

    unsigned totalMovementCost = 0;
    unsigned totalMovements = 0;
};
