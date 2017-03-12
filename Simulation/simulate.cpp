#include <SFML/Graphics.hpp>

#include "../Micromouse/Coordinate.h"
#include "../Micromouse/Direction.h"
#include "../Micromouse/Maze.h"
#include "../Micromouse/Mouse.h"
#include "../Micromouse/Node.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <stdexcept>

const std::string WINDOW_TITLE = "Micromouse simulator";
const float NODE_SIZE = 1. / std::max<float>(Maze::NODE_COLS, Maze::NODE_ROWS);

class CellDrawable : public sf::Transformable, public sf::Drawable {
  public:
    CellDrawable(Maze &maze, CellCoordinate pos) : maze(maze), pos(pos) {
    }

  private:
    sf::VertexArray line(float x1, float y1, float x2, float y2,
                         sf::Color color = sf::Color::White) const {
        sf::VertexArray line(sf::Lines, 2);

        line[0] = sf::Vertex(sf::Vector2f(x1, y1), color);
        line[1] = sf::Vertex(sf::Vector2f(x2, y2), color);
        return line;
    }

    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const {
        states.transform *= getTransform();

        sf::RectangleShape cell(sf::Vector2f(1, 1));
        cell.setFillColor(maze.getNode(pos).evaluated
                              ? sf::Color(255, 255, 150, 20)
                              : sf::Color(255, 255, 255, 10));
        target.draw(cell, states);

        if (maze.isWall(pos, N))
            target.draw(line(0, 0, 1, 0), states);

        if (maze.isWall(pos, S))
            target.draw(line(0, 1, 1, 1), states);

        if (maze.isWall(pos, E))
            target.draw(line(1, 0, 1, 1), states);

        if (maze.isWall(pos, W))
            target.draw(line(0, 0, 0, 1), states);
    }

    Maze &maze;
    CellCoordinate pos;
};

class MazeDrawable : public sf::Transformable, public sf::Drawable {
  public:
    MazeDrawable(Maze &maze) : maze(maze) {
    }

  private:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const {
        states.transform *= getTransform();

        for (int row = 0; row < Maze::CELL_ROWS; row++) {
            for (int col = 0; col < Maze::CELL_COLS; col++) {
                CellCoordinate pos(col, row);
                CellDrawable cell(maze, pos);
                cell.setPosition(nodeVector(pos.toNode() + NW));
                cell.setScale(sf::Vector2f(NODE_SIZE, NODE_SIZE) * 2.f);
                target.draw(cell, states);
            }
        }

        drawPath(target, states, maze.getPath());
    }

    sf::Vector2f nodeVector(NodeCoordinate c) const {
        return sf::Vector2f(.5 + c.x, Maze::NODE_ROWS - c.y - .5) * NODE_SIZE;
    }

    sf::VertexArray line(NodeCoordinate c1, NodeCoordinate c2,
                         sf::Color color = sf::Color::White) const {
        sf::VertexArray line(sf::Lines, 2);
        line[0] = sf::Vertex(nodeVector(c1), color);
        line[1] = sf::Vertex(nodeVector(c2), color);
        return line;
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
};

class Simulator : public sf::RenderWindow {
  public:
    Simulator(Mouse &mouse, Maze &maze)
        : sf::RenderWindow(sf::VideoMode(800, 600), WINDOW_TITLE)
        , maze(maze)
        , mouse(mouse) {
        try {
            mouse.getMaze() = Maze::fromFile("test.maze");
        } catch (const std::exception &e) {
            std::cout << e.what();
        }
    }

    bool keyPress(sf::Keyboard::Key key) {
        if (sf::Keyboard::isKeyPressed(key)) {
            while (sf::Keyboard::isKeyPressed(key)) {
            }
            return true;
        }
        return false;
    }

    void main_loop(void) {
        while (isOpen()) {
            if (keyPress(sf::Keyboard::R)) {
                maze.findPath(CellCoordinate(rand() % Maze::CELL_COLS,
                                             rand() % Maze::CELL_ROWS),
                              CellCoordinate(rand() % Maze::CELL_COLS,
                                             rand() % Maze::CELL_ROWS));
            } else if (keyPress(sf::Keyboard::Return)) {
                maze.findPath(CellCoordinate(0, 0), CellCoordinate(7, 7));
            } else if (keyPress(sf::Keyboard::Space)) {
                maze.generate();
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

    Maze &maze;
    Mouse &mouse;
};

int main() {
    srand(time(0));
    Mouse mouse;
    Simulator simulator(mouse, mouse.getMaze());
    simulator.main_loop();
    return 0;
}
