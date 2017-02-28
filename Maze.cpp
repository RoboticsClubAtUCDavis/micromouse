#include "Maze.h"
#include <stdexcept>
#include <set>
#include <cstdlib> //abs

using namespace std;

const CellCoordinate Maze::CELL_START = CellCoordinate(0,0);
const CellCoordinate  Maze::CELL_FINISH = CellCoordinate(Maze::CELL_COLS / 2, Maze::CELL_ROWS / 2);

void Maze::reset() {
	for (int y = 0; y < NODE_ROWS; y++)
		for (int x = 0; x < NODE_COLS; x++)
			maze[y][x] = Node(NodeCoordinate(x,y));
}



Maze::Maze()
{
	reset();
}

bool Maze::isWall(NodeCoordinate pos) {
    return !maze[pos.y][pos.x];
}

bool Maze::isWall(CellCoordinate pos, Direction dir) {
    auto node = pos.toNode();
    int x = node.x;
    int y = node.y;
    switch (dir) {
        case N:
            y++;
            break;
        case S:
            y--;
            break;
        case E:
            x--;
            break;
        case W:
            x++;
            break;
        default:
            throw std::invalid_argument("Direction must be: N, S, E, W");
    }

    return !maze[y][x];
}

void Maze::setWall(NodeCoordinate pos, bool wall) {
    maze[pos.y][pos.x].exists = !wall;
}

void Maze::setWall(CellCoordinate pos, Direction dir, bool wall) {
    auto node = pos.toNode();
    int x = node.x;
    int y = node.y;
    switch (dir) {
        case N:
            y++;
            break;
        case S:
            y--;
            break;
        case E:
            x--;
            break;
        case W:
            x++;
            break;
        default:
            throw std::invalid_argument("Direction must be: N, S, E, W");
    }

    maze[y][x].exists = !wall;
}


Path Maze::findPath(CellCoordinate start, CellCoordinate end)
{
	// The nodes that still need to be evaluated.
	// Initially insert the start node.
	set<Node *> openNodes;
	openNodes.insert(getNode(start));
	
	// Reset any metadata from previous pathfinding.
	resetNodePathData();
	
	// The start node is 0 distance away.
	getNode(start)->setScores(0, heuristic(start, end));

	// While there are node remaining to be evaluated.
	while (!openNodes.empty())
	{
		// Get the open node with lowest score.
		auto currentNodeItr = openNodes.begin();

		// If the current node is the finish node.
		if ( *currentNodeItr == getNode( end ) )
		{
			// We are done. Construct the path.
			return constructPath(*currentNodeItr);
		}
		
		// Set the current node as evaluated.
		openNodes.erase(currentNodeItr);
		(*currentNodeItr)->evaluated = true;

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
			Node * adjacentNode = getAdjacentNode(*currentNodeItr, direction);

			// If the adjacent node has already been evaluated or does not exist.
			if (adjacentNode->evaluated || !adjacentNode->exists)
			{
				// Ignore the adjacent node.
				continue;
			}

			auto tentativeScore = (*currentNodeItr)->gScore + calculateMovementCost( direction );

			// If adjacent node not in openNodes.
			if (openNodes.find(adjacentNode) == openNodes.end() )
			{
				// Discover a new node.
				openNodes.insert(adjacentNode);
			}
			// If the previous score given to the adjacent node is better.
			else if (tentativeScore >= adjacentNode->gScore)
			{
				// This is not a better path.
				continue;
			}

			// This path is the best so far.
			adjacentNode->previous = *currentNodeItr;
			auto heuristicScore = heuristic((*currentNodeItr)->pos, end);
			(*currentNodeItr)->setScores(tentativeScore, adjacentNode->gScore + heuristicScore);
		}
	}

	throw runtime_error("No path found");
}

Node * Maze::getNode(NodeCoordinate pos)
{
	return &maze[pos.y][pos.x];
}

Node * Maze::getAdjacentNode(Node * node, Direction direction)
{
	return getNode(node->pos + direction);
}

void Maze::resetNodePathData()
{
	for (size_t y = 0; y < CELL_ROWS; y++)
	{
		for (size_t x = 0; x < CELL_COLS; x++)
		{
			Node * node = getNode(NodeCoordinate(x, y));

			if (*node)
			{
				node->resetPathData();
			}
		}
	}
}

unsigned Maze::calculateMovementCost(Direction direction)
{
	switch (direction)
	{
	case N:
	case E:
	case S:
	case W:
		return 100;
	case NE:
	case SE:
	case SW:
	case NW:
		return 141;
	default:
		throw std::invalid_argument("Direction cannot be: NONE");
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

Path Maze::constructPath(Node * end)
{
	Path path;

	const Node * nodeA = end->previous;
	const Node * nodeB = end;

	// while there is more to the path to traverse
	while (nodeA)
	{
		//drawLine( nodeA->pos, nodeB->pos );
		path.push(DirectionVector(nodeA->direction, 1));
		nodeB = nodeA;
		nodeA = nodeA->previous;
	}

	return path;
}
