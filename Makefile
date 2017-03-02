CFLAGS = -g -std=c++11 -Wall -Wextra

all: simulate

Test/test: Test/Test-Direction.o Test/Test-Main.o Direction.o
	${CXX} ${CFLAGS} -o $@ $^

Test/Test-Main.o: Test/Test-Main.cpp
	${CXX} ${CFLAGS} -c -o $@ $<

Test/Test-Direction.o: Test/Test-Direction.cpp Direction.h
	${CXX} ${CFLAGS} -c -o $@ $<

simulate: simulate.o Maze.o Coordinate.o Node.o Direction.o Path.o
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

check: Test/test
	Test/test

clean:
	rm -f simulate Test/test *.o

.PHONY: all clean check
