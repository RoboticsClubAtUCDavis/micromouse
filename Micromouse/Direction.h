#pragma once

enum Direction { N, NE, E, SE, S, SW, W, NW, NONE };

namespace DirOp {
Direction invert(Direction direction);
}

class DirectionVector {
  public:
    DirectionVector();
    DirectionVector(Direction direction, int magnitude);
    DirectionVector invert() const;
    int magnitude;
    Direction direction;
};