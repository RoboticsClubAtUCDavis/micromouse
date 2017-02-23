#include "Maze.h"


void Maze::reset() {
    for (int i = 0; i < NODE_ROWS; i++)
        for (int j = 0; i < NODE_COLS; j++)
            maze[i][j].reset();
}


bool Maze::isWall(Coordinate pos) {
}

bool Maze::isWall(Coordinate pos, Direction dir) {
}

void Maze::setWall(Coordinate pos, bool wall) {
}

void Maze::setWall(Coordinate pos, Direction dir, bool wall) {

}
