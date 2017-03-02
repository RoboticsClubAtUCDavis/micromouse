#pragma once

#include "Direction.h"

class Coordinate {
  protected:
    Coordinate();
    Coordinate(int x, int y);
	public:
    int x;
    int y;
};

class CellCoordinate;

class NodeCoordinate : public Coordinate {
  public:
    NodeCoordinate(int x, int y) : Coordinate(x, y){};
	NodeCoordinate operator+(Direction);
};

class CellCoordinate : public Coordinate {
  public:
    CellCoordinate(int x, int y) : Coordinate(x, y){};
	CellCoordinate operator+(Direction);
	operator NodeCoordinate() const;
    NodeCoordinate toNode(void) const;
};
