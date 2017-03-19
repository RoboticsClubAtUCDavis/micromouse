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

    Maze();

    bool isWall(NodeCoordinate pos);
    bool isWall(CellCoordinate pos, Direction dir);
    bool isBorder(NodeCoordinate pos);
    bool isBorder(Node node);
    bool withinBounds(NodeCoordinate pos);
    bool withinBounds(Node node);

    void setWall(NodeCoordinate pos, bool wall = true);
    void setWall(CellCoordinate pos, Direction dir, bool wall = true);

    bool isExplored(NodeCoordinate pos);
    bool isExplored(CellCoordinate pos, Direction dir);
    void setExplored(NodeCoordinate pos, bool explored = true);
    void setExplored(CellCoordinate pos, Direction dir, bool explored = true);
    
    void generate(int seed = time(NULL));

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

    Node &getNode(NodeCoordinate pos);

  private:
    static bool scoreComparator(const Node *const &lhs, const Node *const &rhs);

    Node &getAdjacentNode(Node *node, Direction direction);

    void resetNodePathData();
    unsigned calculateMovementCost(Direction currentDirection,
                                   Direction nextDirection);
    unsigned heuristic(NodeCoordinate start, NodeCoordinate end);
    void constructPath(Node *start);

    Path path;
    Node maze[NODE_ROWS][NODE_COLS];
};

#endif
