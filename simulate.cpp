#include <SFML/Graphics.hpp>

#include "Coordinate.h"
#include "Direction.h"
#include "Maze.h"
#include "Path.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <stdexcept>

const std::string WINDOW_TITLE = "Micromouse simulator";

class MazeDrawable : public sf::Transformable, public sf::Drawable {
  public:
    MazeDrawable(Maze &maze) : maze(maze) {
        node_size = 1. / std::max<float>(Maze::NODE_COLS, Maze::NODE_ROWS);
    }

  private:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const {
        states.transform *= getTransform();

        for (int row = 0; row < Maze::CELL_ROWS; row++) {
            for (int col = 0; col < Maze::CELL_COLS; col++) {
                drawCell(target, states, CellCoordinate(col, row));
            }
        }

        drawPath(target, states, maze.getPath());
    }

    sf::Vector2f nodeVector(NodeCoordinate c) const {
        return sf::Vector2f(.5 + c.x, Maze::NODE_ROWS - c.y - .5) * node_size;
    }

    sf::VertexArray line(NodeCoordinate c1, NodeCoordinate c2,
                         sf::Color color = sf::Color::White) const {
        sf::VertexArray line(sf::Lines, 2);
        line[0] = sf::Vertex(nodeVector(c1), color);
        line[1] = sf::Vertex(nodeVector(c2), color);
        return line;
    }

    void drawCell(sf::RenderTarget &target, sf::RenderStates states,
                  CellCoordinate pos) const {
        NodeCoordinate node = pos;

        if (maze.isWall(pos, N))
            target.draw(line(node + NW, node + NE), states);

        if (maze.isWall(pos, S))
            target.draw(line(node + SW, node + SE), states);

        if (maze.isWall(pos, E))
            target.draw(line(node + SE, node + NE), states);

        if (maze.isWall(pos, W))
            target.draw(line(node + SW, node + NW), states);

        sf::RectangleShape cell(
            sf::Vector2f(node_size * 2 - .001, node_size * 2 - .001));
        cell.move(nodeVector(node + NW));
        cell.setFillColor(maze.getNode(node).evaluated
                              ? sf::Color(255, 255, 150, 20)
                              : sf::Color(255, 255, 255, 10));
        target.draw(cell, states);
    }

    void drawPath(sf::RenderTarget &target, sf::RenderStates states,
                  const Path &path) const {
        NodeCoordinate node = path.start;
        for (auto &i : path) {
            NodeCoordinate nextNode = node + i;
            target.draw(line(node, nextNode, sf::Color::Green), states);
            node = nextNode;
        }
    }

    Maze &maze;
    float node_size;
};

class Simulator : public sf::RenderWindow {
  public:
    Simulator(void)
        : sf::RenderWindow(sf::VideoMode(800, 600), WINDOW_TITLE), maze() {
        try {
            maze = Maze::fromFile("test.maze");
        } catch (const std::exception &e) {
            std::cout << e.what();
        }
    }

    void main_loop(void) {

        while (isOpen()) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
                maze.findPath(CellCoordinate(rand() % Maze::CELL_COLS,
                                             rand() % Maze::CELL_ROWS),
                              CellCoordinate(rand() % Maze::CELL_COLS,
                                             rand() % Maze::CELL_ROWS),
                              N);

                while (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
                }
            }

            sf::Event event;
            while (pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    close();
                if (event.type == sf::Event::Resized)
                    setView(sf::View(sf::FloatRect(0, 0, event.size.width,
                                                   event.size.height)));
            }

            render();
        }
    }

  private:
    sf::Vector2f mazeSize(void) {
        sf::Vector2u size = getSize();
        float min = std::min(size.x, size.y);
        return sf::Vector2f(min, min);
    }

    void render(void) {
        clear(sf::Color::Black);
        MazeDrawable entity(maze);
        entity.setScale(mazeSize());
        draw(entity);
        display();
    }

    Maze maze;
};

int main() {
    srand(time(0));
    Simulator simulator;
    simulator.main_loop();
    return 0;
}
