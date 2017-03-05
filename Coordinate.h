#pragma once

#include "Direction.h"
#define HASH_CONST 1000
class Coordinate {
  protected:
    Coordinate();
    Coordinate(int x, int y);
    Coordinate(Direction d);
    int hash() const;

  public:
    Coordinate operator*(int a);
    Coordinate operator-(const Coordinate &a);
    Coordinate operator+(const Coordinate &a);
    bool operator<(const Coordinate &a) const;
    bool operator==(const Coordinate &a) const;
    bool operator!=(const Coordinate &a) const;
    int x;
    int y;
};

class CellCoordinate;

class NodeCoordinate : public Coordinate {
  public:
    NodeCoordinate(int x, int y) : Coordinate(x, y){};
    NodeCoordinate(Direction d) : Coordinate(d){};
    bool isCell();
    bool isPost();
    NodeCoordinate operator*(int a);
    NodeCoordinate operator-(NodeCoordinate a);
    NodeCoordinate operator+(NodeCoordinate a);
    NodeCoordinate operator+(Direction direction);
    NodeCoordinate operator+(DirectionVector dirvect);
};

class CellCoordinate : public Coordinate {
  public:
    CellCoordinate(int x, int y) : Coordinate(x, y){};
    CellCoordinate(Direction d) : Coordinate(d){};
    CellCoordinate operator*(int a);
    CellCoordinate operator-(CellCoordinate a);
    CellCoordinate operator+(CellCoordinate a);
    CellCoordinate operator+(Direction direction);
    CellCoordinate operator+(DirectionVector dirvect);
    operator NodeCoordinate() const;
    NodeCoordinate toNode(void) const;
};
