#pragma once
#include <vector>

#include "Coordinate.h"
#include "Direction.h"

class Path {
  public:
    Path() : start(0, 0), cost(0) {
    }
    std::vector<DirectionVector>::const_reverse_iterator begin() const;
    std::vector<DirectionVector>::const_reverse_iterator end() const;
    std::vector<DirectionVector>::size_type size() const;
    void push_back(DirectionVector vec);
    void clear();
    NodeCoordinate start;
    unsigned cost;

  private:
    std::vector<DirectionVector> directions;
};
