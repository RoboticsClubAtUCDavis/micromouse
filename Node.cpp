#include "Node.h"

Node::Node(void) {
    exists = true;
}

Node::operator bool() const {
    return exists;
}
