#include "Maze.h"
#include <stdexcept>

const CellCoordinate Maze::CELL_START = CellCoordinate(0,0);
const CellCoordinate  Maze::CELL_FINISH = CellCoordinate(Maze::CELL_COLS / 2, (Maze::CELL_ROWS / 2);

void Maze::reset() {
	for (int y = 0; y < NODE_ROWS; y++)
		for (int x = 0; x < NODE_COLS; x++)
			maze[y][x] = Node(NodeCoordinate(x, y));
}



Maze::Maze()
{
	reset();
}

bool Maze::isWall(NodeCoordinate pos) {
    return !maze[pos.y][pos.x];
}

bool Maze::isWall(CellCoordinate pos, Direction dir) {
    auto node = pos.toNode();
    int x = node.x;
    int y = node.y;
    switch (dir) {
        case N:
            y++;
            break;
        case S:
            y--;
            break;
        case E:
            x--;
            break;
        case W:
            x++;
            break;
        default:
            throw std::invalid_argument("Direction must be: N, S, E, W");
    }

    return !maze[y][x];
}

void Maze::setWall(NodeCoordinate pos, bool wall) {
    maze[pos.y][pos.x].exists = !wall;
}

void Maze::setWall(CellCoordinate pos, Direction dir, bool wall) {
    auto node = pos.toNode();
    int x = node.x;
    int y = node.y;
    switch (dir) {
        case N:
            y++;
            break;
        case S:
            y--;
            break;
        case E:
            x--;
            break;
        case W:
            x++;
            break;
        default:
            throw std::invalid_argument("Direction must be: N, S, E, W");
    }

    maze[y][x].exists = !wall;
}


Path Maze::findPath(CellCoordinate start, CellCoordinate end)
{




	return Path();
}