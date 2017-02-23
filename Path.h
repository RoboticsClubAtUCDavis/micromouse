#pragma once
#include <stack>
#include "Direction.h"

class Path
{
public:
	Path();

		void push(DirectionVector step);
		DirectionVector peek();
		DirectionVector pop();
		int size();
};

