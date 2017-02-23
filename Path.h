#pragma once
#include <stack>
#include "Direction.h"

class Path
{
public:
	Path();

		void push(DirectionVector step);
		DirectionVector top();
		DirectionVector topPop();
		void pop();
		int size();
private:
	std::stack<DirectionVector> path;	
};

