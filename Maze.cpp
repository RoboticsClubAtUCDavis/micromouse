#include "Maze.h"
#include <stdexcept>

const CellCoordinate Maze::CELL_START = CellCoordinate(0,0);
const CellCoordinate  Maze::CELL_FINISH = CellCoordinate(Maze::CELL_COLS / 2, Maze::CELL_ROWS / 2);

void Maze::reset() {
	for (int y = 0; y < NODE_ROWS; y++) {
		for (int x = 0; x < NODE_COLS; x++) {
			maze[y][x] = Node();

			//Initialize border
			if (x == 0 || y == 0 || y == NODE_ROWS - 1 || x == NODE_COLS - 1)
				setWall(NodeCoordinate(x, y));
		}
	}
}



Maze::Maze()
{
	reset();
}

bool Maze::isWall(NodeCoordinate pos) {
    return !maze[pos.y][pos.x];
}

bool Maze::isWall(CellCoordinate pos, Direction dir) {
	return isWall(pos.toNode() + dir);
}

void Maze::setWall(NodeCoordinate pos, bool wall) {
    maze[pos.y][pos.x].exists = !wall;
}

void Maze::setWall(CellCoordinate pos, Direction dir, bool wall) {
	setWall(pos.toNode() + dir, wall);
}


Path Maze::findPath(CellCoordinate start, CellCoordinate end)
{




	return Path();
}
