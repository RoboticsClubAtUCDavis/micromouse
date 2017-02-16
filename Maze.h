#ifndef MAZE_H
#define MAZE_H

#include "Node.h"
#include "Coordinate.h"
#include "Direction.h"

const int ROWS = 31;
const int COLS = 31;

class Maze {
	public:
		Maze();
		Maze(int rows, int cols);
		bool isWall(Coordinate pos);
		bool isWall(Coordinate pos, Direction dir);
		void setWall(Coordinate pos, Direction dir, bool wall);
		void reset();
	private:
		Node maze[ROWS][COLS];
};

#endif