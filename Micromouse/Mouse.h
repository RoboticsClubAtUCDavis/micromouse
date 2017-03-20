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
    enum MappingStrategy { EXHAUSTIVE, STRAT2, STRAT3 };

#if !defined(__MK66FX1M0__) && !defined(__MK20DX256__)
    friend class Simulator;
    friend class MouseDrawable;

    void stopMaze();
#endif

    Mouse();

    void mapMaze();
    void runMaze();

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
    // encountered. Returns the total number of nodes traveled.
    unsigned move(DirectionVector movement, bool keepGoing, bool useCaution);

    void mapMazeExhaustive();

    void mapMazeStrategy3();

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

    MappingStrategy mappingStrategy = STRAT3;

    bool running;
};
