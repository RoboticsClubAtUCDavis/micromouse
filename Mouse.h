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
    void followPath(const Path &path);
    void move(DirectionVector movement);

    Maze maze;
    Hardware bot;

    NodeCoordinate position;
    Direction facing;

    // In millimeter per second.
    int speed = 100;
};