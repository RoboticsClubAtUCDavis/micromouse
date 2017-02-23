#include "Path.h"
#include <assert.h>


Path::Path()
{
}

void Path::push(DirectionVector step)
{
	path.push(step);
}

DirectionVector Path::top()
{
	assert(!path.empty());
	return path.top();
}
DirectionVector Path::topPop() 
{
	DirectionVector topValue = path.top();
	path.pop();
	return topValue;
}

void Path::pop()
{
	assert(!path.empty());
	path.pop();
}

int Path::size()
{
	return path.size();
}
