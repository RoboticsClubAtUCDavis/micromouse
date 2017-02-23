#include "Coordinate.h"

Coordinate::Coordinate() :
	x(0),
	y(0)
{}

Coordinate::Coordinate(int x, int y) :
	x(x),
	y(y)
{}


CellCoordinate NodeCoordinate::toCell(Direction dir) const {
    // TODO
}


NodeCoordinate CellCoordinate::toNode(Direction dir) const {
    // TODO
}
