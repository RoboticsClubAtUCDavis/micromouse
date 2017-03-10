#include "Direction.h"

DirectionVector::DirectionVector() : magnitude(0), direction(NONE) {
}

DirectionVector::DirectionVector(Direction direction, int magnitude)
    : magnitude(magnitude), direction(direction) {
}

DirectionVector DirectionVector::invert() const {
    return DirectionVector(DirOp::invert(direction), magnitude);
}

Direction DirOp::invert(Direction direction) {
    if (direction == NONE)
        return NONE;
    else
        return static_cast<Direction>((direction + S) % NONE);
}
