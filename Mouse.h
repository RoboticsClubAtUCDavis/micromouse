#pragma once
#include "Coordinate.h"
#include "Direction.h"
#include "Hardware.h"
#include "Maze.h"
#include "Path.h"

class Mouse {
  public:
    Mouse();

    void mapMaze();
    void runMaze();

    // In millimeter per second.
    void setMappingSpeed(unsigned mmps);
    void setRunningSpeed(unsigned mmps);

    void reset();

  private:
    void moveTo(CellCoordinate position);
    void followPath(const Path &path);

    // If `keepGoing` is true the mouse will not stop at the end of the
    // movement. If `useCaution` is true it will stop when a wall is
    // encountered. Returns the total number of nodes traveled.
    unsigned move(DirectionVector movement, bool keepGoing, bool useCaution);

    Maze maze;
    Hardware bot;

    NodeCoordinate position;
    Direction facing;

    // In millimeter per second.
    int speed = 100;
};