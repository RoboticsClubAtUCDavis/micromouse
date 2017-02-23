#ifndef MAZE_H
#define MAZE_H

#include "Coordinate.h"
#include "Direction.h"
#include "Node.h"

class Maze {
  public:
    static const int CELL_ROWS = 16;
    static const int CELL_COLS = 16;
    static const int NODE_ROWS = CELL_ROWS * 2 - 1;
    static const int NODE_COLS = CELL_COLS * 2 - 1;

    bool isWall(NodeCoordinate pos);
    bool isWall(CellCoordinate pos, Direction dir);

    void setWall(NodeCoordinate pos, bool wall);
    void setWall(CellCoordinate pos, Direction dir, bool wall);

    void reset();

  private:
    Node maze[NODE_ROWS][NODE_COLS];
};

#endif
