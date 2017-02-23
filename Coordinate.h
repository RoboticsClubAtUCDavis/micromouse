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
     CellCoordinate toCell(Direction dir) const;
};


class CellCoordinate : public Coordinate {
  public:
     NodeCoordinate toNode(Direction dir) const;
};
