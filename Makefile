CFLAGS = -g

all: simulate Node.o Maze.o Coordinate.o Path.o Direction.o

simulate: simulate.o Maze.o Coordinate.o Node.o Direction.o
	${CXX} ${CFLAGS} -o $@ $^ -lsfml-graphics -lsfml-window -lsfml-system

simulate.o: simulate.cpp Maze.h Coordinate.h Direction.h Node.h
	${CXX} ${CFLAGS} -c -o $@ $<

Node.o: Node.cpp Node.h
	${CXX} ${CFLAGS} -c -o $@ $<

Maze.o: Maze.cpp Maze.h Coordinate.h Direction.h Node.h
	${CXX} ${CFLAGS} -c -o $@ $<

Coordinate.o: Coordinate.cpp Coordinate.h
	${CXX} ${CFLAGS} -c -o $@ $<

Path.o: Path.cpp Path.h
	${CXX} ${CFLAGS} -c -o $@ $<

Direction.o: Direction.cpp Direction.h
	${CXX} ${CFLAGS} -c -o $@ $<

clean:
	rm -f *.o

.PHONY: all clean
