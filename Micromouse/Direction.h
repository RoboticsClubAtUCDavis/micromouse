#pragma once

enum Direction { N, NE, E, SE, S, SW, W, NW, NONE };

namespace DirOp {
Direction invert(Direction direction);
int angleDiffAbs(Direction d1, Direction d2);
int angleDiff(Direction d1, Direction d2);
}

class DirectionVector {
  public:
    DirectionVector();
    DirectionVector(Direction direction, int magnitude);
    DirectionVector invert() const;
    int magnitude;
    Direction direction;
};
