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

int DirOp::angleDiffAbs(Direction d1, Direction d2) {
    return (d2 - d1) * 45;
}

int DirOp::angleDiff(Direction d1, Direction d2) {
    int angle = DirOp::angleDiffAbs(d1, d2);
    if (angle > 180)
        angle -= 360;
    else if (angle < -180)
        angle += 360;
    return angle;
}
