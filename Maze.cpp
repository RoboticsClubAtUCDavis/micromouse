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


Path Maze::findPath(CellCoordinate start, CellCoordinate end, Direction facing )
{
	Node * startNode = getNode(start);

	// The nodes that still need to be evaluated.
	// Initially insert the start node.
	set<Node *> openNodes;
	openNodes.insert(getNode(start));
	
	// Reset any metadata from previous pathfinding.
	resetNodePathData();
	
	// The start node is 0 distance away.
	startNode->setScores(0, heuristic(start, end));
	startNode->direction = facing;

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

			auto tentativeScore = (*currentNodeItr)->gScore + calculateMovementCost( (*currentNodeItr)->direction, direction );

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
	case NE:
	case SE:
	case SW:
	case NW:
		cost = MOVEMENT_COST_DIAGONAL;
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
		throw runtime_error("Path has 180deg turn, this should not occur.");
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
