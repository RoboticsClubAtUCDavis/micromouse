#include "Coordinate.h"

Coordinate::Coordinate() : x(0), y(0) {
}

Coordinate::Coordinate(int x, int y) : x(x), y(y) {
}

NodeCoordinate CellCoordinate::toNode(void) const {
    return NodeCoordinate(x * 2, y * 2);
}
