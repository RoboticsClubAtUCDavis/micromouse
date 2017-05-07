#include "Node.h"
#include <limits>

using namespace std;

Node::Node(void) : pos(NodeCoordinate(0, 0)), exists(false) {
    resetPathData();
}

Node::Node(NodeCoordinate pos) : pos(pos), exists(true) {
    resetPathData();
}

Node::operator bool() const {
    return exists;
}

void Node::resetPathData() {
    gScore = numeric_limits<unsigned>::max();
    fScore = numeric_limits<unsigned>::max();
    next = nullptr;
    evaluated = false;
    // direction = NONE; // No affect on subsequent pathfinding
    touched = false;
}
