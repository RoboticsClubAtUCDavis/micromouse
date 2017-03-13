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
    Coordinate operator*(int a) const;
    Coordinate operator-(const Coordinate &a) const;
    Coordinate operator+(const Coordinate &a) const;
    bool operator<(const Coordinate &a) const;
    bool operator==(const Coordinate &a) const;
    bool operator!=(const Coordinate &a) const;
    int x;
    int y;
};

class CellCoordinate;

class NodeCoordinate : public Coordinate {
  public:
    NodeCoordinate(int x = 0, int y = 0) : Coordinate(x, y){};
    explicit NodeCoordinate(Direction d) : Coordinate(d){};
    bool isCell();
    bool isPost();
    NodeCoordinate operator*(const int a) const;
    NodeCoordinate operator-(const NodeCoordinate a) const;
    NodeCoordinate operator+(const NodeCoordinate a) const;
    NodeCoordinate operator+(const Direction direction) const;
    NodeCoordinate operator+(const DirectionVector dirvect) const;
};

class CellCoordinate : public Coordinate {
  public:
    CellCoordinate(int x = 0, int y = 0) : Coordinate(x, y){};
    explicit CellCoordinate(Direction d) : Coordinate(d){};
    CellCoordinate operator*(const int a) const;
    CellCoordinate operator-(const CellCoordinate a) const;
    CellCoordinate operator+(const CellCoordinate a) const;
    CellCoordinate operator+(const Direction direction) const;
    CellCoordinate operator+(const DirectionVector dirvect) const;
    operator NodeCoordinate() const;
    NodeCoordinate toNode(void) const;
};
