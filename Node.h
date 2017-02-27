#include "Coordinate.h"

class Node {
  public:
    Node(void);
	Node(NodeCoordinate pos);
    explicit operator bool() const;
	void resetPathData();
	void setScores(unsigned gScore, unsigned fScore);

	NodeCoordinate pos;
    bool exists;

	// These are used for pathfinding
	unsigned gScore;
	unsigned fScore;
	Node * previous;
};
