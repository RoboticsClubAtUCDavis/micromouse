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

    void setWall(NodeCoordinate pos, bool wall = true);
    void setWall(CellCoordinate pos, Direction dir, bool wall = true);

    void reset();

	// Returns a path from the `start` coordinate to the `end` coordinate
	// `facing` is the direction the mouse is currently facing. If given
	// paths that start in the same direction will be weighted more heavily.
	Path findPath(CellCoordinate start, CellCoordinate end, Direction facing = NONE);

  private:
	  Node* getNode(NodeCoordinate pos);
	  Node* getAdjacentNode(Node* node, Direction direction);
	  void resetNodePathData();
	  unsigned calculateMovementCost(Direction currentDirection, Direction nextDirection);
	  unsigned heuristic(NodeCoordinate start, NodeCoordinate end);
	  Path constructPath( Node* end );

    Node maze[NODE_ROWS][NODE_COLS];
};

#endif
