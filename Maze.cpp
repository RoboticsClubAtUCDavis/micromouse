#include "Maze.h"
#include <stdexcept>

const CellCoordinate Maze::CELL_START = CellCoordinate(0,0);
const CellCoordinate  Maze::CELL_FINISH = CellCoordinate(Maze::CELL_COLS / 2, (Maze::CELL_ROWS / 2);

void Maze::reset() {
    for (int i = 0; i < NODE_ROWS; i++)
        for (int j = 0; i < NODE_COLS; j++)
            maze[i][j] = Node();
}



bool Maze::isWall(NodeCoordinate pos) {
    return !maze[pos.x][pos.y];
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

    return !maze[x][y];
}

void Maze::setWall(NodeCoordinate pos, bool wall) {
    maze[pos.x][pos.y].exists = !wall;
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

    maze[x][y].exists = !wall;
}


Path Maze::findPath(CellCoordinate start, CellCoordinate end)
{




	return Path();
}