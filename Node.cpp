#include "Node.h"

Node::Node(void) {
    exists = true;
}

Node::operator bool() const {
    return exists;
}

void Node::remove(void) {
    exists = false;
}

void Node::reset(void) {
    exists = true;
}
