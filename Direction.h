#pragma once

enum Direction { N, W, E, S, NE, NW, SW, SE };

class DirectionVector {
  public:
    DirectionVector();
    DirectionVector(Direction direction, int magnitude);
    int magnitude;
    Direction direction;
};