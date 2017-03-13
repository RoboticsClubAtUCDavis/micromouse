#include "Mouse.h"
#include <Arduino.h>
#include <cstdio>

Mouse::Mouse() {
    pinMode(13, OUTPUT);
    digitalWrite(13, HIGH);
    Serial.printf("Mouse Created!");
}

void Mouse::mapMaze() {
    switch (mappingStrategy) {
        case Mouse::EXHAUSTIVE:
            mapMazeExhaustive();
            break;
        case Mouse::STRAT2:
            mapMazeStrategy2();
            break;
        case Mouse::STRAT3:
            // TODO
            break;
        default:
            break;
    }
}

void Mouse::runMaze() {
    // TODO
}

void Mouse::mapMazeStrategy2() {
    if (position != CellCoordinate(0, 0))
        reset();
    Node currentNode = Node(position);
    currentNode.direction = facing;
    Path mapPath = Path();
    mapPath.start = currentNode.pos;
    maze.setNodeVisited(currentNode);
    while (!maze.isGoal(position)) {
        seeWalls();
        if (!maze.isWall(position +
                         facing)) { // go straight as much as possible
            move(DirectionVector(facing, 2), true, true);
            currentNode.next = &Node(position);
            currentNode = *currentNode.next;
            currentNode.direction = facing;
        } else {
            if (maze.isNodeVisited(position +
                                   getRelativeDirection(Relation::FRONT)) &&
                maze.isNodeVisited(position +
                                   getRelativeDirection(Relation::LEFT)) &&
                maze.isNodeVisited(position +
                                   getRelativeDirection(Relation::RIGHT))) {
                // back track
				maze.setNodeVisited(currentNode);
            } else {
				NodeCoordinate toCenter = maze.MAZE_CENTER.toNode() - position;
				Direction go;
				if (abs(toCenter.x) > abs(toCenter.y)) { // horizontal
					if (toCenter.x > 0) { // East
						go = E;
					}
					else if(toCenter.x < 0) { // West
						go = W;
					}
				}
				if (abs(toCenter.y) > abs(toCenter.x)) { // vertical
					if (toCenter.y > 0) { // North
						go = N;
					}
					else if (toCenter.y < 0) { // South
						go = S;
					}
				}
				Direction movement = NONE;
				while (movement != NONE) {
					if (go == facing || go == DirOp::invert(facing) || maze.isNodeVisited(position + go)) {
						go = DirOp::rotateCounterClockwise(go);
					}
					else movement = go;
				}
				move(DirectionVector(go, 2), true, true);
				currentNode.next = &Node(position);
				currentNode = *currentNode.next;
				currentNode.direction = go;
			}
        }
    }
}

void Mouse::seeWalls() {
    for (Relation i = Relation::LEFT; i != Relation::NO_RELATION;
         i = static_cast<Relation>(i + 1)) {
		if (bot.isWall(i)) {
			maze.setNodeVisited(position + getRelativeDirection(i));
			maze.setWall(position + getRelativeDirection(i));
		}
    }
}

Direction Mouse::getRelativeDirection(Relation relation) {
    switch (relation) {
        case Relation::FRONT:
            return facing;
        case Relation::LEFT:
            return DirOp::rotateCounterClockwise(facing);
        case Relation::RIGHT:
            return DirOp::rotateClockwise(facing);
    }
}

void Mouse::setMappingSpeed(unsigned mmps) {
    mappingSpeed = mmps;
}

void Mouse::setRunningSpeed(unsigned mmps) {
    runningSpeed = mmps;
}

void Mouse::setMappingStrategy(MappingStrategy strategy) {
    mappingStrategy = strategy;
}

void Mouse::reset() {
    // TODO
}

void Mouse::moveTo(CellCoordinate position) {
    (void)position;
    // TODO
}

void Mouse::followPath(const Path &path) {
    (void)path;
    // TODO
}

unsigned Mouse::move(DirectionVector movement, bool keepGoing,
                     bool useCaution) {
    (void)movement;
    (void)keepGoing;
    (void)useCaution;
    // TODO
    return 0;
}

void Mouse::mapMazeExhaustive() {
    // TODO
}
