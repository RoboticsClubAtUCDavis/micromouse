#include <SFML/Graphics.hpp>

#include "../Micromouse/Coordinate.h"
#include "../Micromouse/Direction.h"
#include "../Micromouse/Maze.h"
#include "../Micromouse/Mouse.h"
#include "../Micromouse/Node.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <mutex>
#include <stdexcept>
#include <thread>

const std::string WINDOW_TITLE = "Micromouse simulator";
const float NODE_SIZE = 1. / std::max<float>(Maze::NODE_COLS, Maze::NODE_ROWS);

sf::Vector2f nodeVector(NodeCoordinate c) {
    return sf::Vector2f(.5 + c.x, Maze::NODE_ROWS - c.y - .5) * NODE_SIZE;
}

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

class MouseDrawable : public sf::Transformable, public sf::Drawable {
  public:
    MouseDrawable(Mouse &mouse) : mouse(mouse) {
    }

  private:
    Mouse &mouse;

    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const {
        states.transform *= getTransform();
        sf::ConvexShape polygon;
        polygon.setPointCount(3);
        polygon.setPoint(0, sf::Vector2f(0.5f, 0.0f));
        polygon.setPoint(1, sf::Vector2f(0.2f, 1.0f));
        polygon.setPoint(2, sf::Vector2f(0.8f, 1.0f));
        polygon.setOrigin(sf::Vector2f(0.5f, 0.5f));

        {
            polygon.setPosition(nodeVector(mouse.position));

            polygon.setRotation(mouse.facing * 45.0f);
        }
        polygon.setFillColor(sf::Color(107, 190, 255));
        // polygon.setOrigin(sf::Vector2f(0.0f, 0.0f));
        polygon.setScale(sf::Vector2f(NODE_SIZE, NODE_SIZE) * 1.0f);
        target.draw(polygon, states);
    }
};

class Simulator : public sf::RenderWindow {
  public:
    Simulator(Mouse &mouse, sf::ContextSettings settings)
        : sf::RenderWindow(sf::VideoMode(800, 600), WINDOW_TITLE,
                           sf::Style::Default, settings)
        , mouse(mouse) {
        try {
            mouse.maze = Maze::fromFile("test.maze");
        } catch (const std::exception &e) {
            std::cout << e.what();
        }

        this->setFramerateLimit(60);
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
                //mouse.maze.findPath(CellCoordinate(rand() % Maze::CELL_COLS,
                //                                   rand() % Maze::CELL_ROWS),
                //                    CellCoordinate(rand() % Maze::CELL_COLS,
                //                                   rand() % Maze::CELL_ROWS));
            } else if (keyPress(sf::Keyboard::Return)) {
                //mouse.maze.findPath(CellCoordinate(0, 0), CellCoordinate(7, 7));
            } else if (keyPress(sf::Keyboard::Space)) {
                //mouse.maze.generate();
            }

            sf::Event event;
            while (pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    close();
                if (event.type == sf::Event::Resized)
                    setView(sf::View(sf::FloatRect(0, 0, event.size.width,
                                                   event.size.height)));
            }

            std::lock_guard<std::mutex> lock(mtx);
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
        MazeDrawable entity(mouse.maze);
        MouseDrawable mouseEntity(mouse);
        entity.setScale(mazeSize());
        mouseEntity.setScale(mazeSize());
        draw(entity);
        draw(mouseEntity);
        display();
    }

    Mouse &mouse;
};

void startSimulation(Mouse *mouse) {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    Simulator simulation(*mouse, settings);
    simulation.main_loop();
}

int main() {
    srand(time(0));

    Mouse mouse;

    std::thread simulation(startSimulation, &mouse);
    mouse.runMaze();

    simulation.join();
    return 0;
}
