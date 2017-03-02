#include "Coordinate.h"

Coordinate::Coordinate() : x(0), y(0) {
}

Coordinate::Coordinate(int x, int y) : x(x), y(y) {
}

CellCoordinate::operator NodeCoordinate() const
{
	return NodeCoordinate(x * 2 + 1, y * 2 + 1);
}

NodeCoordinate CellCoordinate::toNode(void) const {
    return NodeCoordinate(x * 2 + 1, y * 2 + 1);
}

NodeCoordinate NodeCoordinate::operator+(Direction direction) {
	int horizontal = x;
	int vertical = y;
	switch (direction) {
	case N:
		vertical++;
		break;
	case S:
		vertical--;
		break;
	case E:
		horizontal++;
		break;
	case W:
		horizontal--;
		break;
	case NE:
		vertical++;
		horizontal++;
		break;
	case NW:
		vertical++;
		horizontal--;
		break;
	case SE:
		vertical--;
		horizontal++;
		break;
	case SW:
		vertical--;
		horizontal--;
		break;
	default:
		break;
	}
	return NodeCoordinate(horizontal, vertical);
}

CellCoordinate CellCoordinate::operator+(Direction direction) {
	int horizontal = x;
	int vertical = y;
	switch (direction) {
	case N:
		vertical++;
		break;
	case S:
		vertical--;
		break;
	case E:
		horizontal--;
		break;
	case W:
		horizontal++;
		break;
	case NE:
		vertical++;
		horizontal++;
		break;
	case NW:
		vertical++;
		horizontal++;
		break;
	case SE:
		vertical--;
		horizontal++;
		break;
	case SW:
		vertical--;
		horizontal--;
		break;
	default:
		break;
	}
	return CellCoordinate(horizontal, vertical);
}
