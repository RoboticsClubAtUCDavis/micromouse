#ifndef MAZE_H
#define MAZE_H

#include "Coordinate.h"
#include "Direction.h"
#include "Node.h"
#include "Path.h"

//#define MAZE_DIAGONALS

class Maze {
  public:
    static const int CELL_ROWS = 16;
    static const int CELL_COLS = 16;
    static const int NODE_ROWS = CELL_ROWS * 2 + 1;
    static const int NODE_COLS = CELL_COLS * 2 + 1;

	static const CellCoordinate CELL_START;
	static const CellCoordinate CELL_FINISH;

	static const unsigned MOVEMENT_COST = 100;
	static const unsigned MOVEMENT_COST_DIAGONAL = 141;

	Maze();

    bool isWall(NodeCoordinate pos);
    bool isWall(CellCoordinate pos, Direction dir);

    void setWall(NodeCoordinate pos, bool wall);
    void setWall(CellCoordinate pos, Direction dir, bool wall);

    void reset();

	Path findPath(CellCoordinate start = CELL_START, CellCoordinate end = CELL_FINISH);

  private:
	  Node* getNode(NodeCoordinate pos);
	  Node* getAdjacentNode(Node* node, Direction direction);
	  void resetNodePathData();
	  unsigned calculateMovementCost(Direction direction);
	  unsigned heuristic(NodeCoordinate start, NodeCoordinate end);

    Node maze[NODE_ROWS][NODE_COLS];
};

#endif
