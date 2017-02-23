all: Node.o Maze.o Coordinate.o Path.o Direction.o

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
