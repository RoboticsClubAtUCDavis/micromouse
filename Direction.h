#pragma once

enum Direction { N, NE, E, SE, S, SW, W, NW, NONE };

class DirectionVector {
  public:
    DirectionVector();
    DirectionVector(Direction direction, int magnitude);
    int magnitude;
    Direction direction;
};