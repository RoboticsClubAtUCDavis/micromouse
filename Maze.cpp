#include "Maze.h"
#include <stdexcept>
#include <set>
#include <cstdlib> //abs
#include <fstream>
#include <stdexcept>
#include <iostream>

#define MAZE_DIAGONALS

using namespace std;

const CellCoordinate Maze::CELL_START = CellCoordinate(0,0);
const CellCoordinate  Maze::CELL_FINISH = CellCoordinate(Maze::CELL_COLS / 2, Maze::CELL_ROWS / 2);

void Maze::reset() {
	for (int y = 0; y < NODE_ROWS; y++) {
		for (int x = 0; x < NODE_COLS; x++) {
			maze[y][x] = Node(NodeCoordinate(x,y));

			//Initialize border
			if (x == 0 || y == 0 || y == NODE_ROWS - 1 || x == NODE_COLS - 1)
				setWall(NodeCoordinate(x, y));
		}
	}

	path.clear();
}

Maze Maze::fromFile(std::string fileName) {
    Maze maze;

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

    return maze;
}

Maze::Maze()
{
	reset();
}

bool Maze::isWall(NodeCoordinate pos) {
    return !maze[pos.y][pos.x];
}

bool Maze::isWall(CellCoordinate pos, Direction dir) {
	return isWall(pos.toNode() + dir);
}

void Maze::setWall(NodeCoordinate pos, bool wall) {
    maze[pos.y][pos.x].exists = !wall;
}

void Maze::setWall(CellCoordinate pos, Direction dir, bool wall) {
	setWall(pos.toNode() + dir, wall);
}


void Maze::findPath(CellCoordinate start, CellCoordinate end, Direction facing )
{
	// Pathfinding is done from end to start
	Node& endNode = getNode(end);

	// The nodes that still need to be evaluated.
	// Initially insert the end node.
	set<Node *> openNodes;
	openNodes.insert(&endNode);
	
	// Reset any metadata from previous pathfinding.
	resetNodePathData();
	
	// The end node is 0 distance away.
	endNode.gScore = 0;
	endNode.fScore = heuristic(end, start);
	endNode.direction = facing;

	// While there are node remaining to be evaluated.
	while (!openNodes.empty())
	{
		// Get the open node with lowest score.
		auto currentNodeItr = openNodes.begin();
		auto currentNode = *currentNodeItr;

		// If the current node is the finish node.
		if ( currentNode == &getNode( start ) )
		{
			// We are done. Construct the path.
			constructPath(*currentNodeItr);
			return;
		}
		
		// Set the current node as evaluated.
		// The iterator is incremented so we can no longer use it.
		openNodes.erase(currentNodeItr);
		currentNode->evaluated = true;

		// For each node adjacent to the current node.
		for (
			Direction direction = Direction::N;
			direction != Direction::NONE;
#ifdef MAZE_DIAGONALS
			direction = static_cast<Direction>(direction + 1)
#else
			direction = static_cast<Direction>(direction + 2)
#endif // MAZE_DIAGONALS
			)
		{
			Node& adjacentNode = getAdjacentNode(currentNode, direction);

			// If the adjacent node has already been evaluated or does not exist.
			if (adjacentNode.evaluated || !adjacentNode.exists)
			{
				// Ignore the adjacent node.
				continue;
			}

			auto tentativeScore = currentNode->gScore + calculateMovementCost(currentNode->direction, direction );

			// If adjacent node not in openNodes.
			if (openNodes.find(&adjacentNode) == openNodes.end() )
			{
				// Discover a new node.
				openNodes.insert(&adjacentNode);
			}
			// If the previous score given to the adjacent node is better.
			else if (tentativeScore >= adjacentNode.gScore)
			{
				// This is not a better path.
				continue;
			}

			// This path is the best so far.
			adjacentNode.next = currentNode;
			adjacentNode.direction = static_cast<Direction>(( direction + S ) % NONE); // TODO move to function/overload
			auto heuristicScore = heuristic(currentNode->pos, start);
			currentNode->gScore = tentativeScore;
			currentNode->fScore = adjacentNode.gScore + heuristicScore;
		}
	}

	throw runtime_error("No path found");
}

Node& Maze::getNode(NodeCoordinate pos)
{
	if (pos.x < 0 || pos.x >= NODE_COLS || pos.y < 0 || pos.y >= NODE_ROWS)
		throw out_of_range("Coordinate out of range.");
	return maze[pos.y][pos.x];
}

Node& Maze::getAdjacentNode(Node * node, Direction direction)
{
	return getNode(node->pos + direction);
}

void Maze::resetNodePathData()
{
	for (size_t y = 0; y < NODE_ROWS; y++)
	{
		for (size_t x = 0; x < NODE_COLS; x++)
		{
			Node& node = getNode(NodeCoordinate(x, y));

			if (node)
			{
				node.resetPathData();
			}
		}
	}
}

unsigned Maze::calculateMovementCost(Direction currentDirection, Direction nextDirection)
{
	unsigned cost;
	switch (nextDirection)
	{
	case N:
	case E:
	case S:
	case W:
		cost = MOVEMENT_COST;
		break;
	case NE:
	case SE:
	case SW:
	case NW:
		cost = MOVEMENT_COST_DIAGONAL;
		break;
	default:
		throw std::invalid_argument("Direction cannot be: NONE");
	}

	// We are not given what direction we are currently facing so no penalty can be given.
	if (currentDirection == NONE)
	{
		return cost;
	}

	// Penalize turns.
	switch ( abs(nextDirection - currentDirection) )
	{
	case 0:
		// Goiing straight has no additional cost.
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
		// We probably never want to do this it is a very difficult turn to make.
		// This is an approximation. 
		return cost + MOVEMENT_COST * 2;
	case 4:
		// path 180 turn. It is guaranteed not to be the shortest so just return cost.
		return cost;
	default:
		throw runtime_error("This should be unreachable");
	}
}

unsigned Maze::heuristic(NodeCoordinate start, NodeCoordinate end)
{
#ifdef MAZE_DIAGONALS
	int deltaX = abs(start.x - end.x);
	int deltaY = abs(start.y - end.y);

	if (deltaX>deltaY)
		return (deltaX - deltaY) * MOVEMENT_COST + deltaY * MOVEMENT_COST_DIAGONAL;
	else
		return (deltaY - deltaX) * MOVEMENT_COST + deltaX * MOVEMENT_COST_DIAGONAL;
#else
	return abs(start.x - end.x) + abs(start.y - end.y);
#endif // MAZE_DIAGONALS
}

void Maze::constructPath(Node * start)
{
	path.clear();
	path.start = start->pos;

	// while there is more to the path to traverse
	for (const Node *i = start; i; i = i->next)
	{
		path.push_back(DirectionVector(i->direction, 1));
	}
}

const Path& Maze::getPath() const
{
	return path;
}
