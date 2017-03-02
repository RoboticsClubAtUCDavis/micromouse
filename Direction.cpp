#include "Direction.h"

DirectionVector::DirectionVector() :
	magnitude(0),
	direction(NONE)
{}

DirectionVector::DirectionVector(Direction direction, int magnitude) :
	magnitude(magnitude),
	direction(direction)
{}
