all: simulate Node.o Maze.o Coordinate.o Path.o Direction.o

simulate: simulate.o Maze.o Coordinate.o Node.o Direction.o
	${CXX} -o $@ $^ -lsfml-graphics -lsfml-window -lsfml-system

simulate.o: simulate.cpp Maze.h Coordinate.h Direction.h Node.h
	${CXX} -c -o $@ $<

Node.o: Node.cpp Node.h
	${CXX} -c -o $@ $<

Maze.o: Maze.cpp Maze.h Coordinate.h Direction.h Node.h
	${CXX} -c -o $@ $<

Coordinate.o: Coordinate.cpp Coordinate.h
	${CXX} -c -o $@ $<

Path.o: Path.cpp Path.h
	${CXX} -c -o $@ $<

Direction.o: Direction.cpp Direction.h
	${CXX} -c -o $@ $<

clean:
	rm -f *.o

.PHONY: all clean
