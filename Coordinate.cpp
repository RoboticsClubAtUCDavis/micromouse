#include "Coordinate.h"
#include <cmath>

Coordinate::Coordinate() : x(0), y(0) {
}

Coordinate::Coordinate(int x, int y) : x(x), y(y) {
}

Coordinate::Coordinate(Direction d)
{
	switch (d) {
		case N:
			*this = Coordinate(0, 1);
			break;
		case S:
			*this = Coordinate(0, -1);
			break;
		case W:
			*this = Coordinate(-1, 0);
			break;
		case E:
			*this = Coordinate(1, 0);
			break;
		case NE:
			*this = Coordinate(1, 1);
			break;
		case NW:
			*this = Coordinate(-1, 1);
			break;
		case SE:
			*this = Coordinate(1, -1);
			break;
		case SW:
			*this = Coordinate(-1, -1);
			break;
	}
}

int Coordinate::hash() const
{
	return x * HASH_CONST + y;
}

Coordinate Coordinate::operator-(const Coordinate& a) {
    return Coordinate(x - a.x, y - a.y);
}

Coordinate Coordinate::operator+(const Coordinate& a) {
    return Coordinate(x + a.x, y + a.y);
}

bool Coordinate:: operator<(const Coordinate& a) const
{
	return hash() < a.hash();
}

bool Coordinate::operator==(const Coordinate& a) const
{
	return a.x == x && a.y == y;
}

bool Coordinate::operator!=(const Coordinate& a) const
{
	return !(*this == a);
}


CellCoordinate::operator NodeCoordinate() const {
    return NodeCoordinate(x * 2 + 1, y * 2 + 1);
}

NodeCoordinate CellCoordinate::toNode(void) const {
    return NodeCoordinate(x * 2 + 1, y * 2 + 1);
}

bool NodeCoordinate::isCell() {
    return x % 2 && y % 2;
}

bool NodeCoordinate::isPost() {
    return !(x % 2 || y % 2);
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
    return CellCoordinate(horizontal, vertical);
}

NodeCoordinate NodeCoordinate::operator+(DirectionVector dirvect) {
    int horizontal = x;
    int vertical = y;
    switch (dirvect.direction) {
        case N:
            vertical += dirvect.magnitude;
            break;
        case S:
            vertical -= dirvect.magnitude;
            break;
        case E:
            horizontal += dirvect.magnitude;
            break;
        case W:
            horizontal -= dirvect.magnitude;
            break;
        case NE:
            vertical += dirvect.magnitude;
            horizontal += dirvect.magnitude;
            break;
        case NW:
            vertical += dirvect.magnitude;
            horizontal -= dirvect.magnitude;
            break;
        case SE:
            vertical -= dirvect.magnitude;
            horizontal += dirvect.magnitude;
            break;
        case SW:
            vertical -= dirvect.magnitude;
            horizontal -= dirvect.magnitude;
            break;
        default:
            break;
    }
    return NodeCoordinate(horizontal, vertical);
}

CellCoordinate CellCoordinate::operator+(DirectionVector dirvect) {
    int horizontal = x;
    int vertical = y;
    switch (dirvect.direction) {
        case N:
            vertical += dirvect.magnitude;
            break;
        case S:
            vertical -= dirvect.magnitude;
            break;
        case E:
            horizontal += dirvect.magnitude;
            break;
        case W:
            horizontal -= dirvect.magnitude;
            break;
        case NE:
            vertical += dirvect.magnitude;
            horizontal += dirvect.magnitude;
            break;
        case NW:
            vertical += dirvect.magnitude;
            horizontal -= dirvect.magnitude;
            break;
        case SE:
            vertical -= dirvect.magnitude;
            horizontal += dirvect.magnitude;
            break;
        case SW:
            vertical -= dirvect.magnitude;
            horizontal -= dirvect.magnitude;
            break;
        default:
            break;
    }
    return CellCoordinate(horizontal, vertical);
}
