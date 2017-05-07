#pragma once

#include "Coordinate.h"

class Node {
  public:
    Node(void);
    Node(NodeCoordinate pos);
    explicit operator bool() const;
    void resetPathData();

    NodeCoordinate pos;
    bool exists;

    // True := The state of the node has been observed
    bool explored = false;

    // True := The bot has entered this node
    bool visited = false;

    // These are used for pathfinding
    unsigned gScore;
    unsigned fScore;
    Node *next;
    Direction direction;
    bool evaluated;
    bool touched;
};
