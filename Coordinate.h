#pragma once

#include "Direction.h"

class Coordinate {
  public:
    Coordinate();
    Coordinate(int x, int y);
    int x;
    int y;
};


class CellCoordinate;


class NodeCoordinate : public Coordinate {
  public:
    NodeCoordinate(int x, int y) : Coordinate(x, y) {};
};


class CellCoordinate : public Coordinate {
  public:
     CellCoordinate(int x, int y) : Coordinate(x, y) {};
     NodeCoordinate toNode(void) const;
};
