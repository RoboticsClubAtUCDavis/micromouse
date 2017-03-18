#pragma once
#include "Coordinate.h"
#include "Direction.h"
#include "Hardware.h"
#include "Maze.h"
#include "Path.h"

#if !defined(__MK66FX1M0__) && !defined(__MK20DX256__)
#include <mutex>
extern std::mutex mtx;

class Simulator;
class MouseDrawable;
#endif

class Mouse {
  public:
    enum MappingStrategy { EXHAUSTIVE, STRAT2, STRAT3 };

#if !defined(__MK66FX1M0__) && !defined(__MK20DX256__)
    friend class Simulator;
    friend class MouseDrawable;
#endif

    Mouse();

    void mapMaze();
    void runMaze();
    void stopMaze();

    // In millimeter per second.
    void setMappingSpeed(unsigned mmps);
    void setRunningSpeed(unsigned mmps);

    void setMappingStrategy(MappingStrategy strategy);

    void reset();

  private:
    void moveTo(CellCoordinate position);
    void followPath(const Path &path);

    // If `keepGoing` is true the mouse will not stop at the end of the
    // movement. If `useCaution` is true it will stop when a wall is
    // encountered. Returns the total number of nodes traveled.
    unsigned move(DirectionVector movement, bool keepGoing, bool useCaution);

    void mapMazeExhaustive();

    Maze maze;
    Hardware bot;

    NodeCoordinate position;
    Direction facing;

    // In millimeter per second.
    int mappingSpeed = 100;
    int runningSpeed = 100;

    MappingStrategy mappingStrategy = EXHAUSTIVE;

    bool running;
};
