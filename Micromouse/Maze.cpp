#include "Maze.h"
#include <Arduino.h>
#include <algorithm>
#include <assert.h>
#include <cstdlib> //abs
#include <limits>
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

bool Maze::isWall(NodeCoordinate pos) const {
    return !getNode(pos);
}

bool Maze::isWall(CellCoordinate pos, Direction dir) const {
    return isWall(pos.toNode() + dir);
}

bool Maze::isWall(NodeCoordinate pos, Direction dir) const {
    return isWall(pos + dir);
}

bool Maze::isWall(Relation rel, NodeCoordinate pos, Direction facing) const {
    return isWall(pos + DirOp::relToDir(rel, facing));
}

void Maze::setWall(NodeCoordinate pos, bool wall) {
    getNode(pos).exists = !wall;
}

void Maze::setWall(CellCoordinate pos, Direction dir, bool wall) {
    setWall(pos.toNode() + dir, wall);
}

void Maze::setWall(NodeCoordinate pos, Direction dir, bool wall) {
    setWall(pos + dir, wall);
}

void Maze::setWall(Relation rel, NodeCoordinate pos, Direction facing,
                   bool wall) {
    setWall(pos + DirOp::relToDir(rel, facing), wall);
}

bool Maze::isExplored(NodeCoordinate pos) const {
    return getNode(pos).explored;
}

bool Maze::isExplored(CellCoordinate pos, Direction dir) const {
    return isExplored(pos.toNode() + dir);
}

bool Maze::isExplored(NodeCoordinate pos, Direction dir) const {
    return isExplored(pos + dir);
}

bool Maze::isExplored(Relation rel, NodeCoordinate pos,
                      Direction facing) const {
    return isExplored(pos + DirOp::relToDir(rel, facing));
}

void Maze::setExplored(NodeCoordinate pos, bool explored) {
    getNode(pos).explored = explored;
}

void Maze::setExplored(CellCoordinate pos, Direction dir, bool explored) {
    setExplored(pos.toNode() + dir, explored);
}

void Maze::setExplored(NodeCoordinate pos, Direction dir, bool explored) {
    setExplored(pos + dir, explored);
}

void Maze::setExplored(Relation rel, NodeCoordinate pos, Direction facing,
                       bool explored) {
    setExplored(pos + DirOp::relToDir(rel, facing), explored);
}

bool Maze::isBorder(NodeCoordinate pos) {
    return pos.x == 0 || pos.y == 0 || pos.y == NODE_ROWS - 1 ||
           pos.x == NODE_COLS - 1;
}

bool Maze::withinBounds(NodeCoordinate pos) {
    return pos.x >= 0 && pos.x < NODE_COLS && pos.y >= 0 && pos.y < NODE_ROWS;
}

Maze Maze::generate(uint16_t seed) {
    Maze maze;
    Serial.printf("Generate Maze - seed: %4X\n", seed);
    srand(seed);

    while (true) {
        vector<CellCoordinate> backtrackStack;
        set<CellCoordinate> unvisited;
        backtrackStack.reserve(CELL_COLS * CELL_ROWS);

        for (int y = 0; y < CELL_ROWS; y++) {
            for (int x = 0; x < CELL_COLS; x++) {
                unvisited.insert(CellCoordinate(x, y));
                maze.setWall(CellCoordinate(x, y), N);
                maze.setWall(CellCoordinate(x, y), E);
                maze.setWall(CellCoordinate(x, y), W);
                maze.setWall(CellCoordinate(x, y), S);
            }
        }

        CellCoordinate currentCell = Maze::CELL_START;

        unvisited.erase(CELL_FINISH);
        unvisited.erase(CELL_FINISH + S);
        unvisited.erase(CELL_FINISH + W);
        unvisited.erase(CELL_FINISH + SW);

        unvisited.erase(currentCell);

        vector<Direction> neighbors;
        while (unvisited.size() > 0) {
            for (auto direction : DIRECTIONS_CARDINAL) {
                if (withinBounds(currentCell + direction) &&
                    unvisited.find(currentCell + direction) !=
                        unvisited.end()) {
                    neighbors.push_back(direction);
                }
            }

            if (neighbors.size() > 0) {
                Direction wallDir = neighbors[rand() % neighbors.size()];
                CellCoordinate chosen = currentCell + wallDir;
                backtrackStack.push_back(currentCell);
                maze.setWall(currentCell.toNode() + wallDir, false);
                currentCell = chosen;
                unvisited.erase(currentCell);
            } else {
                currentCell = backtrackStack.back();
                backtrackStack.pop_back();
            }
            neighbors.clear();
        }

        maze.setWall(NODE_FINISH + N, false);
        maze.setWall(NODE_FINISH + E, false);
        maze.setWall(NODE_FINISH + W, false);
        maze.setWall(NODE_FINISH + S, false);

        NodeCoordinate goalEntrances[] = {
            NodeCoordinate(SE) + E, NodeCoordinate(SE) + S,
            NodeCoordinate(SW) + S, NodeCoordinate(SW) + W,
            NodeCoordinate(NE) + E, NodeCoordinate(NE) + N,
            NodeCoordinate(NW) + N, NodeCoordinate(NW) + W};

        maze.setWall(Maze::NODE_FINISH + goalEntrances[rand() % 8], false);
        maze.findPath(NODE_START, NODE_FINISH);

        if (maze.path.size() > 150)
            break;

        // maze.reset();
    }

    float currentSimulationSpeed = SIMULATION_SPEED;
    SIMULATION_SPEED = numeric_limits<float>::infinity();

    int removalAttempts = 30;
    // 30 + 3d20 expected value = 61.5
    removalAttempts += rand() % 20 + 1;
    removalAttempts += rand() % 20 + 1;
    removalAttempts += rand() % 20 + 1;

    unsigned minPathLength = rand() % 30 + 90;

    const NodeCoordinateList finishWalls{
        NodeCoordinate(NODE_FINISH.x - 2, NODE_FINISH.y - 1),
        NodeCoordinate(NODE_FINISH.x - 2, NODE_FINISH.y + 1),
        NodeCoordinate(NODE_FINISH.x + 2, NODE_FINISH.y - 1),
        NodeCoordinate(NODE_FINISH.x + 2, NODE_FINISH.y + 1),
        NodeCoordinate(NODE_FINISH.x - 1, NODE_FINISH.y + 2),
        NodeCoordinate(NODE_FINISH.x + 1, NODE_FINISH.y + 2),
        NodeCoordinate(NODE_FINISH.x - 1, NODE_FINISH.y - 2),
        NodeCoordinate(NODE_FINISH.x + 1, NODE_FINISH.y - 2)};

    for (int removals = 0; removals < removalAttempts; removals++) {
        NodeCoordinate removal = maze.removeRandomWall();

        // If it failed to find a wall to remove.
        if (removal == NodeCoordinate()) {
            continue;
        }

        // If the wall it found was a finish wall.
        if (maze.countWalls(finishWalls) < 7) {
            maze.setWall(removal);
            continue;
        }

        // If the wall removed makes the path too short.
        maze.findPath(NODE_START, NODE_FINISH);
        if (maze.path.size() < minPathLength) {
            maze.setWall(removal);
            continue;
        }

        // Serial.printf("Removed Wall (%2i, %2i), Shortest Path: %u\n",
        // removal.x,
        //              removal.y, maze.path.size());
    }

    SIMULATION_SPEED = currentSimulationSpeed;

    return maze;
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
    std::unique_lock<std::mutex> lock(mtx, std::defer_lock);
    // A bit of a hack, didn't feel like refactoring to give each maze its own
    // mutex.
    if (SIMULATION_SPEED != numeric_limits<float>::infinity()) {
        lock.lock();
    }
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

#ifdef MAZE_DIAGONALS
        // For each node adjacent to the current node. All directions
        for (auto direction : DIRECTIONS) {
#else
        // For each node adjacent to the current node. Cardinal directions
        for (auto direction : DIRECTIONS_CARDINAL) {
#endif // MAZE_DIAGONALS
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

const Node &Maze::getNode(NodeCoordinate pos) const {
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

NodeCoordinate Maze::removeRandomWall() {
    // A better way to implement this is to collect all the walls that could be
    // removed and then select randomly from that. This works well enough since
    // it is only needed for simulation.

    for (size_t tries = 0; tries < 1000; tries++) {

        // Pick a random x coordinate.
        NodeCoordinate candidate{rand() % (NODE_COLS - 2) + 1, 0};

        if (candidate.x % 2 == 0) {
            // Pick a random y coordinate such that it where a wall could be.
            candidate.y = 2 * (rand() % ((NODE_ROWS - 1) / 2)) + 1;

            NodeCoordinateList a{
                NodeCoordinate(candidate.x - 1, candidate.y + 1),
                NodeCoordinate(candidate.x + 0, candidate.y + 2),
                NodeCoordinate(candidate.x + 1, candidate.y + 1)};

            NodeCoordinateList b{
                NodeCoordinate(candidate.x - 1, candidate.y - 1),
                NodeCoordinate(candidate.x + 0, candidate.y - 2),
                NodeCoordinate(candidate.x + 1, candidate.y - 1)};

            // if it is a wall and removing doesnt violate the requirements for
            // a valid maze.
            if (isWall(candidate) && countWalls(a) > 0 && countWalls(b) > 0) {
                setWall(candidate, false);
                return candidate;
            }
        } else {
            // Pick a random y coordinate such that it where a wall could be.
            candidate.y = 2 * (rand() % ((NODE_ROWS - 3) / 2)) + 2;

            NodeCoordinateList a{
                NodeCoordinate(candidate.x - 1, candidate.y + 1),
                NodeCoordinate(candidate.x - 2, candidate.y + 0),
                NodeCoordinate(candidate.x - 1, candidate.y - 1)};

            NodeCoordinateList b{
                NodeCoordinate(candidate.x + 1, candidate.y + 1),
                NodeCoordinate(candidate.x + 2, candidate.y + 0),
                NodeCoordinate(candidate.x + 1, candidate.y - 1)};

            // if it is a wall and removing doesnt violate the requirements for
            // a valid maze.
            if (isWall(candidate) && countWalls(a) > 0 && countWalls(b) > 0) {
                setWall(candidate, false);
                return candidate;
            }
        }
    }

    return NodeCoordinate();
}

unsigned Maze::countWalls(NodeCoordinateList coords) const {
    unsigned count = 0;
    for (const auto &c : coords) {
        if (withinBounds(c) && isWall(c))
            count++;
    }
    return count;
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