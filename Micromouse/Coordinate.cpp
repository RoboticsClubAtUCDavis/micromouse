#include "Coordinate.h"
#include <cmath>

Coordinate::Coordinate() : x(0), y(0) {
}

Coordinate::Coordinate(int x, int y) : x(x), y(y) {
}

Coordinate::Coordinate(Direction d) {
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
        default:
            *this = Coordinate(0, 0);
            break; // will never reach this state
    }
}

int Coordinate::hash() const {
    return x * HASH_CONST + y;
}

Coordinate Coordinate::operator*(int a) {
    return Coordinate(a * x, a * y);
}

Coordinate Coordinate::operator-(const Coordinate &a) {
    return Coordinate(x - a.x, y - a.y);
}

Coordinate Coordinate::operator+(const Coordinate &a) {
    return Coordinate(x + a.x, y + a.y);
}

bool Coordinate::operator<(const Coordinate &a) const {
    return hash() < a.hash();
}

bool Coordinate::operator==(const Coordinate &a) const {
    return a.x == x && a.y == y;
}

bool Coordinate::operator!=(const Coordinate &a) const {
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

NodeCoordinate NodeCoordinate::operator*(int a) {
    return NodeCoordinate(a * x, a * y);
}

NodeCoordinate NodeCoordinate::operator-(NodeCoordinate a) {
    return NodeCoordinate(x - a.x, y - a.y);
}

NodeCoordinate NodeCoordinate::operator+(NodeCoordinate a) {
    return NodeCoordinate(x + a.x, y + a.y);
}

NodeCoordinate NodeCoordinate::operator+(Direction direction) {
    return *this + NodeCoordinate(direction);
}

CellCoordinate CellCoordinate::operator*(int a) {
    return CellCoordinate(a * x, a * y);
}

CellCoordinate CellCoordinate::operator-(CellCoordinate a) {
    return CellCoordinate(x - a.x, y - a.y);
}

CellCoordinate CellCoordinate::operator+(CellCoordinate a) {
    return CellCoordinate(x + a.x, y + a.y);
}

CellCoordinate CellCoordinate::operator+(Direction direction) {
    return *this + CellCoordinate(direction);
}

NodeCoordinate NodeCoordinate::operator+(DirectionVector dirvect) {
    return *this + NodeCoordinate(dirvect.direction) * dirvect.magnitude;
}

CellCoordinate CellCoordinate::operator+(DirectionVector dirvect) {
    return *this + CellCoordinate(dirvect.direction) * dirvect.magnitude;
}
