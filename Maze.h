#ifndef MAZE_H
#define MAZE_H

#include "Coordinate.h"
#include "Direction.h"
#include "Node.h"
#include "Path.h"

class Maze {
  public:
    static const int CELL_ROWS = 16;
    static const int CELL_COLS = 16;
    static const int NODE_ROWS = CELL_ROWS * 2 + 1;
    static const int NODE_COLS = CELL_COLS * 2 + 1;

	static const CellCoordinate CELL_START;
	static const CellCoordinate CELL_FINISH;

	Maze();

    bool isWall(NodeCoordinate pos);
    bool isWall(CellCoordinate pos, Direction dir);

    void setWall(NodeCoordinate pos, bool wall = true);
    void setWall(CellCoordinate pos, Direction dir, bool wall = true);

    void reset();

	Path findPath(CellCoordinate start = CELL_START, CellCoordinate end = CELL_FINISH);

  private:
    Node maze[NODE_ROWS][NODE_COLS];
};

#endif
