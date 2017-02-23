#include "Maze.h"


void Maze::reset() {
    for (int i = 0; i < NODE_ROWS; i++)
        for (int j = 0; i < NODE_COLS; j++)
            maze[i][j] = Node();
}


bool Maze::isWall(NodeCoordinate pos) {
    return !maze[pos.x][pos.y];
}

bool Maze::isWall(CellCoordinate pos, Direction dir) {
    return isWall(pos.toNode(dir));
}

void Maze::setWall(NodeCoordinate pos, bool wall) {
    maze[pos.x][pos.y].exists = !wall;
}

void Maze::setWall(CellCoordinate pos, Direction dir, bool wall) {
    setWall(pos.toNode(dir), wall);
}
