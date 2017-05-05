#include "Maze.h"
#include <algorithm>
#include <assert.h>
#include <cstdlib> //abs
#include <set>
#include <stdexcept>
#include <vector>

#if !defined(__MK66FX1M0__) && !defined(__MK20DX256__)
#include "../Simulation/simulate.h"
#include <chrono>
#include <fstream>
#include <iostream>
#include <mutex>
#include <stdexcept>
#include <thread>
#endif

//#define MAZE_DIAGONALS

using namespace std;

const CellCoordinate Maze::CELL_START = CellCoordinate(0, 0);
const CellCoordinate Maze::CELL_FINISH =
    CellCoordinate(Maze::CELL_COLS / 2, Maze::CELL_ROWS / 2);
const NodeCoordinate Maze::NODE_START = CellCoordinate(0, 0);
const NodeCoordinate Maze::NODE_FINISH =
    NodeCoordinate(Maze::NODE_COLS / 2, Maze::NODE_COLS / 2);

Maze::Maze() {
    reset();
}

void Maze::reset() {
    for (int y = 0; y < NODE_ROWS; y++) {
        for (int x = 0; x < NODE_COLS; x++) {
            NodeCoordinate pos = NodeCoordinate(x, y);
            maze[y][x] = Node(pos);

            // Initialize border
            if (isBorder(pos) || pos.isPost())
                setWall(pos);
        }
    }
    setWall(NodeCoordinate(NODE_COLS / 2, NODE_ROWS / 2), false);
    path.clear();
}

bool Maze::isWall(NodeCoordinate pos) {
    return !getNode(pos);
}

bool Maze::isWall(CellCoordinate pos, Direction dir) {
    return isWall(pos.toNode() + dir);
}

void Maze::setWall(NodeCoordinate pos, bool wall) {
    getNode(pos).exists = !wall;
}

void Maze::setWall(CellCoordinate pos, Direction dir, bool wall) {
    setWall(pos.toNode() + dir, wall);
}

bool Maze::isExplored(NodeCoordinate pos) {
    return getNode(pos).explored;
}

bool Maze::isExplored(CellCoordinate pos, Direction dir) {
    return isExplored(pos.toNode() + dir);
}

void Maze::setExplored(NodeCoordinate pos, bool explored) {
    getNode(pos).explored = explored;
}

void Maze::setExplored(CellCoordinate pos, Direction dir, bool explored) {
    setExplored(pos.toNode() + dir, explored);
}

bool Maze::isBorder(NodeCoordinate pos) {
    return pos.x == 0 || pos.y == 0 || pos.y == NODE_ROWS - 1 ||
           pos.x == NODE_COLS - 1;
}

bool Maze::withinBounds(NodeCoordinate pos) {
    return pos.x >= 0 && pos.x < NODE_COLS && pos.y >= 0 && pos.y < NODE_ROWS;
}

void Maze::generate(int seed) {
    reset();
    srand(seed);
    vector<CellCoordinate> backtrackStack;
    set<CellCoordinate> unvisited;
    backtrackStack.reserve(CELL_COLS * CELL_ROWS);

    for (int y = 0; y < CELL_ROWS; y++) {
        for (int x = 0; x < CELL_COLS; x++) {
            unvisited.insert(CellCoordinate(x, y));
            setWall(CellCoordinate(x, y).toNode() + N);
            setWall(CellCoordinate(x, y).toNode() + E);
            setWall(CellCoordinate(x, y).toNode() + W);
            setWall(CellCoordinate(x, y).toNode() + S);
        }
    }
    CellCoordinate currentCell = CellCoordinate(0, 0);
    CellCoordinate centerFour =
        CellCoordinate(CELL_COLS / 2 - 1, CELL_ROWS / 2 - 1);
    unvisited.erase(centerFour);
    unvisited.erase(centerFour + N);
    unvisited.erase(centerFour + E);
    unvisited.erase(centerFour + NE);

    unvisited.erase(currentCell);
    vector<Direction> neighbors;
    neighbors.reserve(4);
    while (unvisited.size() > 0) {
        for (Direction direction = Direction::N; direction != Direction::NONE;
             direction = static_cast<Direction>(direction + 2)) {
            if (withinBounds(currentCell + direction) &&
                unvisited.find(currentCell + direction) != unvisited.end()) {
                neighbors.push_back(direction);
            }
        }

        if (neighbors.size() > 0) {
            Direction wallDir = neighbors[rand() % neighbors.size()];
            CellCoordinate chosen = currentCell + wallDir;
            backtrackStack.push_back(currentCell);
            setWall(currentCell.toNode() + wallDir, false);
            currentCell = chosen;
            unvisited.erase(currentCell);
        } else {
            currentCell = backtrackStack.back();
            backtrackStack.pop_back();
        }
        neighbors.clear();
    }
    NodeCoordinate centerNode = centerFour.toNode() + NE;
    setWall(centerNode + N, false);
    setWall(centerNode + E, false);
    setWall(centerNode + W, false);
    setWall(centerNode + S, false);
    NodeCoordinate goalEntrances[] = {
        NodeCoordinate(SE) + E, NodeCoordinate(SE) + S, NodeCoordinate(SW) + S,
        NodeCoordinate(SW) + W, NodeCoordinate(NE) + E, NodeCoordinate(NE) + N,
        NodeCoordinate(NW) + N, NodeCoordinate(NW) + W};
    setWall(centerNode + goalEntrances[rand() % 8], false);
}

Maze Maze::fromFile(std::string fileName) {
    Maze maze;

#if !defined(__MK66FX1M0__) && !defined(__MK20DX256__)
    try {
        ifstream file(fileName.c_str());
        if (!file.good())
            throw runtime_error("Could not load file");

        int x = 0;
        int y = NODE_ROWS - 1;

        while (file) {
            char c;
            while (file && file.get(c)) {
                if (c == '*') {
                    maze.getNode(NodeCoordinate(x, y)).exists = false;
                    x++;
                    cout << "* ";
                } else if (c == ' ') {
                    x++;
                    cout << "  ";
                }

                if (x >= NODE_COLS) {
                    x = 0;
                    y--;
                    cout << endl;
                }
            }
        }
    } catch (const exception &e) {
        cout << e.what();
    }
#endif

    return maze;
}

bool Maze::scoreComparator(const Node *const &lhs, const Node *const &rhs) {
    return lhs->fScore < rhs->fScore;
}

void Maze::findPath(NodeCoordinate start, NodeCoordinate end, bool exploredOnly,
                    Direction facing) {
    findPath(start, NodeCoordinateList(1, end), exploredOnly, facing);
}

void Maze::findPath(NodeCoordinate start, const NodeCoordinateList &ends,
                    bool exploredOnly, Direction facing) {
#if !defined(__MK66FX1M0__) && !defined(__MK20DX256__)
    std::unique_lock<std::mutex> lock(mtx);
#endif

    if (ends.empty())
        return;

    // Pathfinding is done from start to end.
    Node &startNode = getNode(start);
    startNode.direction = facing;

    // The nodes that still need to be evaluated.
    // Initially insert the start node.
    vector<Node *> openNodes;
    openNodes.push_back(&startNode);

    // Reset any metadata from previous pathfinding.
    resetNodePathData();

    // The start node is 0 distance away.
    startNode.gScore = 0;

    // Can only use the heuristic with a single end point.
    if (ends.size() == 1) {
        startNode.fScore = heuristic(start, ends[0]);
    } else {
        startNode.fScore = 0;
    }

    // While there are nodes remaining to be evaluated.
    while (!openNodes.empty()) {
        // Get the open node with lowest score.
        auto currentNodeItr = min_element(openNodes.begin(), openNodes.end(),
                                          Maze::scoreComparator);
        auto currentNode = *currentNodeItr;
        path.cost = currentNode->fScore;

        // If the current node is any of the end nodes.
        for (auto &i : ends) {
            if (currentNode == &getNode(i)) {
                // We are done. Construct the path.
                constructPath(currentNode);
#if !defined(__MK66FX1M0__) && !defined(__MK20DX256__)
                if (SIMULATION_SPEED < 1000.0f) {
                    lock.unlock();
                    std::this_thread::sleep_for(std::chrono::milliseconds(
                        long(1000 / SIMULATION_SPEED)));
                }
#endif
                return;
            }
        }

        // Set the current node as evaluated.
        // The iterator is incremented so we can no longer use it.
        openNodes.erase(currentNodeItr);
        currentNode->evaluated = true;

        // For each node adjacent to the current node.
        for (Direction direction = Direction::N; direction != Direction::NONE;
#ifdef MAZE_DIAGONALS
             direction = static_cast<Direction>(direction + 1)
#else
             direction = static_cast<Direction>(direction + 2)
#endif // MAZE_DIAGONALS
                 ) {
            Node &adjacentNode = getAdjacentNode(currentNode, direction);

            // If the adjacent node has already been evaluated or does not exist
            // or it is not explored and it requires explored.
            if (adjacentNode.evaluated || !adjacentNode.exists ||
                (exploredOnly && !adjacentNode.explored)) {
                // Ignore the adjacent node.
                continue;
            }

            auto tentativeScore =
                currentNode->gScore +
                calculateMovementCost(currentNode->direction, direction);

            // If adjacent node not in openNodes.
            if (find(openNodes.begin(), openNodes.end(), &adjacentNode) ==
                openNodes.end()) {
                // Discover a new node.
                openNodes.push_back(&adjacentNode);
            }
            // If the previous score given to the adjacent node is better.
            else if (tentativeScore >= adjacentNode.gScore) {
                // This is not a better path.
                continue;
            }

            // This path is the best so far.
            adjacentNode.next = currentNode;
            adjacentNode.direction = direction;
            adjacentNode.touched = true;

            unsigned heuristicScore = 0;
            // Can only use the heuristic with a single end point.
            if (ends.size() == 1) {
                heuristicScore = heuristic(adjacentNode.pos, ends[0]);
            }

            adjacentNode.gScore = tentativeScore;
            adjacentNode.fScore = adjacentNode.gScore + heuristicScore;
        }

#if !defined(__MK66FX1M0__) && !defined(__MK20DX256__)
        if (SIMULATION_SPEED < 10.0f) {
            lock.unlock();
            std::this_thread::sleep_for(
                std::chrono::microseconds(long(5000 / SIMULATION_SPEED)));
            lock.lock();
        }
#endif
    }
#if !defined(__MK66FX1M0__) && !defined(__MK20DX256__)
    throw runtime_error("No path found");
#endif
}

Node &Maze::getNode(NodeCoordinate pos) {
#if !defined(__MK66FX1M0__) && !defined(__MK20DX256__)
    if (!withinBounds(pos))
        throw out_of_range("Coordinate out of range.");
#endif
    return maze[pos.y][pos.x];
}

Node &Maze::getAdjacentNode(Node *node, Direction direction) {
    return getNode(node->pos + direction);
}

void Maze::resetNodePathData() {
    for (size_t y = 0; y < NODE_ROWS; y++) {
        for (size_t x = 0; x < NODE_COLS; x++) {
            Node &node = getNode(NodeCoordinate(x, y));

            if (node) {
                node.resetPathData();
            }
        }
    }
}

unsigned Maze::calculateMovementCost(Direction currentDirection,
                                     Direction nextDirection,
                                     unsigned distance) {
    unsigned cost;
    switch (nextDirection) {
        case N:
        case E:
        case S:
        case W:
            cost = MOVEMENT_COST * distance;
            break;
        case NE:
        case SE:
        case SW:
        case NW:
            cost = MOVEMENT_COST_DIAGONAL * distance;
            break;
        default:
            cost = 0;
    }

    // We are not given what direction we are currently facing so no penalty can
    // be given.
    if (currentDirection == NONE || nextDirection == NONE) {
        return cost;
    }

    // Penalize turns.
    switch (abs(nextDirection - currentDirection)) {
        case 0:
            // Going straight has no additional cost.
            return cost;
        case 1:
        case 7:
            // Turning 45deg costs moving a quarter cell.
            // This is an approximation.
            return cost + MOVEMENT_COST / 4;
        case 2:
        case 6:
            // Turning 90deg costs moving a half cell.
            // This is an approximation.
            return cost + MOVEMENT_COST / 2;
        case 3:
        case 5:
            // Turning 135deg costs moving two cells.
            // We probably never want to do this it is a very difficult turn to
            // make. This is an approximation.
            return cost + MOVEMENT_COST * 2;
        case 4:
            // Turning 180deg costs moving two cells.
            // This is an approximation.
            // This can occur if the mouse is initially facing the opposite
            // direction.
            return cost + MOVEMENT_COST * 2;
        default:
#if !defined(__MK66FX1M0__) && !defined(__MK20DX256__)
            throw runtime_error("This should be unreachable");
#endif
            return cost;
            break;
    }
}

unsigned Maze::heuristic(NodeCoordinate start, NodeCoordinate end) {
#ifdef MAZE_DIAGONALS
    int deltaX = abs(start.x - end.x);
    int deltaY = abs(start.y - end.y);

    if (deltaX > deltaY)
        return (deltaX - deltaY) * MOVEMENT_COST +
               deltaY * MOVEMENT_COST_DIAGONAL;
    else
        return (deltaY - deltaX) * MOVEMENT_COST +
               deltaX * MOVEMENT_COST_DIAGONAL;
#else
    return (abs(start.x - end.x) + abs(start.y - end.y)) * MOVEMENT_COST;
#endif // MAZE_DIAGONALS
}

void Maze::constructPath(Node *start) {
    path.clear();
    path.cost = start->gScore;

    const Node *i;
    // while there is more to the path to traverse
    for (i = start; i->next; i = i->next) {
        path.push_back(DirectionVector(i->direction, 1));
    }

    path.start = i->pos;
}

const Path &Maze::getPath() const {
    return path;
}

void Maze::findNodeCoordPairs(NodeCoordinateList &coordList) {
    // This could be optimized by combining it with `Maze::constructPath` but I
    // made it its own function for clarity.

    // Require that the path start node is explored.
    assert(isExplored(path.start));

    // These are used to detect the transition between explored and unexplored
    // nodes if the edge detectors are not equal then an edge is present.
    bool edgeDetectorA;
    bool edgeDetectorB = true;

#if !defined(__MK66FX1M0__) && !defined(__MK20DX256__)
    std::lock_guard<std::mutex> lock(mtx);
#endif

    NodeCoordinate posA = path.start;
    NodeCoordinate posB;

    for (auto &i : path) {
        posB = posA;
        posA = posA + i.direction;

        edgeDetectorA = getNode(posA).explored;

        if (edgeDetectorA != edgeDetectorB) {
            if (edgeDetectorB) {
                coordList.push_back(posB);
            } else {
                coordList.push_back(posA);
            }
        }

        edgeDetectorB = edgeDetectorA;
    }

    // List must be even size since we assume they are pairs.
    // Effectively requires that the path end node is explored.
    assert(coordList.size() % 2 == 0);
}

void Maze::closeExcessFinishNodes() {
#if !defined(__MK66FX1M0__) && !defined(__MK20DX256__)
    std::lock_guard<std::mutex> lock(mtx);
#endif
    for (int y = -2; y <= 2; y++) {
        for (int x = -2; x <= 2; x++) {
            if (x >= -1 && x <= 1 && y >= -1 && y <= 1)
                continue;

            NodeCoordinate pos = Maze::NODE_FINISH + NodeCoordinate(x, y);

            if (!isExplored(pos)) {
                setWall(pos);
                setExplored(pos);
            }
        }
    }
}