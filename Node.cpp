#include "Node.h"
#include <limits>

using namespace std;

Node::Node(void):
	pos(NodeCoordinate(0,0)),
	exists(false)
{}

Node::Node(NodeCoordinate pos):
	pos(pos),
	exists(true)
{}

Node::operator bool() const {
    return exists;
}

void Node::resetPathData()
{
	gScore = numeric_limits<unsigned>::max();
	fScore = numeric_limits<unsigned>::max();
	previous = nullptr;
	evaluated = false;
	direction = NONE;
}

void Node::setScores(unsigned gScore, unsigned fScore)
{
	this->gScore = gScore;
	this->fScore = fScore;
}