#include <SFML/Graphics.hpp>

#include "../Micromouse/Coordinate.h"
#include "../Micromouse/Direction.h"
#include "../Micromouse/Maze.h"
#include "../Micromouse/Mouse.h"
#include "../Micromouse/Node.h"
#include "simulate.h"
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

        enum States { DEFAULT, EXPLORED, EVALUATED, EXP_AND_EVAL };

        int state = 0;
        state |= maze.getNode(pos).evaluated;
        state <<= 1;
        state |= maze.isExplored(pos);

        sf::Color c;
        switch (state) {
            case EXPLORED:
                c = sf::Color(150, 255, 255, 40);
                break;
            case EVALUATED:
                c = sf::Color(255, 255, 150, 40);
                break;
            case EXP_AND_EVAL:
                c = sf::Color(255, 170, 170, 35);
                break;
            case DEFAULT:
            default:
                c = sf::Color(255, 255, 255, 5);
                break;
        }

        cell.setFillColor(c);
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

        drawPath(target, states, maze.getPath(), maze);
    }

    sf::VertexArray line(NodeCoordinate c1, NodeCoordinate c2,
                         sf::Color color = sf::Color::White) const {
        sf::VertexArray line(sf::Lines, 2);
        line[0] = sf::Vertex(nodeVector(c1), color);
        line[1] = sf::Vertex(nodeVector(c2), color);
        return line;
    }

    void drawPath(sf::RenderTarget &target, sf::RenderStates states,
                  const Path &path, Maze &maze) const {

        enum States { UNEXPLORED, EXPLORED };

        NodeCoordinate node = path.start;

        for (auto &i : path) {
            NodeCoordinate nextNode = node + i;

            int state = 0;
            state |= maze.getNode(node).explored;

            sf::Color color;
            switch (state) {
                case EXPLORED:
                    color = sf::Color::Green;
                    break;
                case UNEXPLORED:
                default:
                    color = sf::Color(255, 225, 100);
                    break;
            }

            target.draw(line(node, nextNode, color), states);
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
        sf::ConvexShape triangleBody;
        triangleBody.setPointCount(3);
        triangleBody.setPoint(0, sf::Vector2f(0.5f, 0.0f));
        triangleBody.setPoint(1, sf::Vector2f(0.2f, 1.0f));
        triangleBody.setPoint(2, sf::Vector2f(0.8f, 1.0f));
        triangleBody.setOrigin(sf::Vector2f(0.5f, 0.5f));
        triangleBody.setPosition(nodeVector(mouse.position));
        triangleBody.setRotation(mouse.facing * 45.0f);
        triangleBody.setFillColor(sf::Color(107, 190, 255));
        triangleBody.setScale(sf::Vector2f(NODE_SIZE, NODE_SIZE) * 1.0f);
        target.draw(triangleBody, states);
    }
};

class Simulator : public sf::RenderWindow {
  public:
    Simulator(Mouse &mouse)
        : sf::RenderWindow(sf::VideoMode(800, 600), WINDOW_TITLE,
                           sf::Style::Default, sf::ContextSettings(0, 0, 8))
        , mouse(mouse) {
        try {
            mouse.virtualMaze = Maze::fromFile("2.maze");
        } catch (const std::exception &e) {
            std::cout << e.what();
        }

        this->setFramerateLimit(120);
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
        // Wait until after the mouse maze has been initialized by the
        // simulation to start the thread.
        mouse_thread = std::thread(&Mouse::runMaze, &mouse);

        while (isOpen()) {
            if (keyPress(sf::Keyboard::R)) {
                // These destroy the path the the mouse is iterating over.
                // Not thread safe.
                // mouse.maze.findPath(CellCoordinate(rand() % Maze::CELL_COLS,
                //                                   rand() % Maze::CELL_ROWS),
                //                    CellCoordinate(rand() % Maze::CELL_COLS,
                //                                   rand() % Maze::CELL_ROWS));
            } else if (keyPress(sf::Keyboard::Return)) {
                // mouse.maze.findPath(CellCoordinate(0, 0), CellCoordinate(7,
                // 7));
            } else if (keyPress(sf::Keyboard::Space)) {
                // mouse.maze.generate();
            } else if (keyPress(sf::Keyboard::Up)) {
                std::unique_lock<std::mutex> lock(mtx);
                SIMULATION_SPEED *= 1.5f;
            } else if (keyPress(sf::Keyboard::Down)) {
                std::unique_lock<std::mutex> lock(mtx);
                SIMULATION_SPEED /= 1.5f;
            }

            sf::Event event;
            while (pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    mouse.stopMaze();
                    mouse_thread.join();
                    close();
                }
                if (event.type == sf::Event::Resized)
                    setView(sf::View(sf::FloatRect(0, 0, event.size.width,
                                                   event.size.height)));
            }

            if (SIMULATION_SPEED < 1000.0f)
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
        std::unique_lock<std::mutex> lock(mtx, std::defer_lock);

        clear(sf::Color::Black);

        MazeDrawable entity(mouse.maze);
        MouseDrawable mouseEntity(mouse);
        entity.setScale(mazeSize());
        mouseEntity.setScale(mazeSize());

        lock.lock();
        draw(entity);
        draw(mouseEntity);
        lock.unlock();

        // Display includes the delay needed to achieved the set framerate.
        // We don't want the mutex locked for the long delay.
        display();
    }

    Mouse &mouse;
    std::thread mouse_thread;
};

int main() {
    srand(time(0));
    Mouse mouse;
    Simulator simulator(mouse);
    simulator.main_loop();
    return 0;
}
