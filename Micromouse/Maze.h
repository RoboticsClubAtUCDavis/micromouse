#ifndef MAZE_H
#define MAZE_H

#include "Coordinate.h"
#include "Direction.h"
#include "Node.h"
#include "Path.h"
#include <ctime>
#include <string>
#include <vector>

//#define MAZE_DIAGONALS

typedef std::vector<NodeCoordinate> NodeCoordinateList;

class Maze {
  public:
    static const int CELL_ROWS = 16;
    static const int CELL_COLS = 16;
    static const int NODE_ROWS = CELL_ROWS * 2 + 1;
    static const int NODE_COLS = CELL_COLS * 2 + 1;
    static const CellCoordinate CELL_START;
    static const CellCoordinate CELL_FINISH;
    static const NodeCoordinate NODE_START;
    static const NodeCoordinate NODE_FINISH;
    static const unsigned MOVEMENT_COST = 100;
    static const unsigned MOVEMENT_COST_DIAGONAL = 141;

    static Maze fromFile(std::string fileName);
    static Maze generate(int seed = time(NULL));

    static bool withinBounds(NodeCoordinate pos);
    static bool isBorder(NodeCoordinate pos);

    static unsigned calculateMovementCost(Direction currentDirection,
                                          Direction nextDirection,
                                          unsigned distance = 1);

    Maze();

    bool isWall(NodeCoordinate pos) const;
    bool isWall(CellCoordinate pos, Direction dir) const;
    bool isWall(NodeCoordinate pos, Direction dir) const;
    bool isWall(Relation rel, NodeCoordinate pos, Direction facing) const;

    void setWall(NodeCoordinate pos, bool wall = true);
    void setWall(CellCoordinate pos, Direction dir, bool wall = true);
    void setWall(NodeCoordinate pos, Direction dir, bool wall = true);
    void setWall(Relation rel, NodeCoordinate pos, Direction facing,
                 bool wall = true);

    bool isExplored(NodeCoordinate pos) const;
    bool isExplored(CellCoordinate pos, Direction dir) const;
    bool isExplored(NodeCoordinate pos, Direction dir) const;
    bool isExplored(Relation rel, NodeCoordinate pos, Direction facing) const;

    void setExplored(NodeCoordinate pos, bool explored = true);
    void setExplored(CellCoordinate pos, Direction dir, bool explored = true);
    void setExplored(NodeCoordinate pos, Direction dir, bool explored = true);
    void setExplored(Relation rel, NodeCoordinate pos, Direction facing,
                     bool explored = true);

    void reset();

    // Returns a path from the `start` coordinate to the `end` coordinate.
    // `facing` is the direction the mouse is currently facing. If given
    // paths that start in the same direction will be weighted more heavily.
    void findPath(NodeCoordinate start, NodeCoordinate end,
                  bool exploredOnly = false, Direction facing = NONE);

    // Same as the other `findPath` but if multiple end points are given the
    // path to the closest is found.
    void findPath(NodeCoordinate start, const NodeCoordinateList &ends,
                  bool exploredOnly = false, Direction facing = NONE);

    const Path &getPath() const;

    // Finds pairs of `NodeCoordinate` along the current `path` that mark the
    // transitions from explored to unexplored and unexlpored to explored
    // `Nodes`. The `NodeCoordinate` of the explored node is always the one
    // used. Requires the start and end of the path to be explored.
    // I could have made the pairs explicit but chose not to to make the
    // overloading of `findPath` cleaner.
    void findNodeCoordPairs(NodeCoordinateList &nodeList);

    // After the entrance to the finish has been found this can be used to set
    // the remaining unexplored nodes to walls.
    void closeExcessFinishNodes();

	Node &getNode(NodeCoordinate pos);
	const Node &getNode(NodeCoordinate pos) const;

  private:
    static bool scoreComparator(const Node *const &lhs, const Node *const &rhs);

    Node &getAdjacentNode(Node *node, Direction direction);

    void resetNodePathData();
    unsigned heuristic(NodeCoordinate start, NodeCoordinate end);
    void constructPath(Node *start);

	NodeCoordinate removeRandomWall();
	unsigned countWalls(NodeCoordinateList coords) const;

    Path path;
    Node maze[NODE_ROWS][NODE_COLS];
};

#endif
