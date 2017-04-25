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
bool colorGradient = true;

sf::Vector2f nodeVector(NodeCoordinate c) {
    return sf::Vector2f(.5 + c.x, Maze::NODE_ROWS - c.y - .5) * NODE_SIZE;
}

void HSVtoRGB(float &fR, float &fG, float &fB, float fH, float fS, float fV) {
    float fC = fV * fS; // Chroma
    float fHPrime = fmod(fH / 60.0, 6);
    float fX = fC * (1 - fabs(fmod(fHPrime, 2) - 1));
    float fM = fV - fC;

    if (0 <= fHPrime && fHPrime < 1) {
        fR = fC;
        fG = fX;
        fB = 0;
    } else if (1 <= fHPrime && fHPrime < 2) {
        fR = fX;
        fG = fC;
        fB = 0;
    } else if (2 <= fHPrime && fHPrime < 3) {
        fR = 0;
        fG = fC;
        fB = fX;
    } else if (3 <= fHPrime && fHPrime < 4) {
        fR = 0;
        fG = fX;
        fB = fC;
    } else if (4 <= fHPrime && fHPrime < 5) {
        fR = fX;
        fG = 0;
        fB = fC;
    } else if (5 <= fHPrime && fHPrime < 6) {
        fR = fC;
        fG = 0;
        fB = fX;
    } else {
        fR = 0;
        fG = 0;
        fB = 0;
    }

    fR += fM;
    fG += fM;
    fB += fM;
}

class CellDrawable : public sf::Transformable, public sf::Drawable {
  public:
    CellDrawable(Maze &maze, Maze &virtualMaze, CellCoordinate pos)
        : maze(maze), virtualMaze(virtualMaze), pos(pos) {
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

        float r, g, b, h, s = 1.0f, v = 1.0f;
        HSVtoRGB(r, g, b,
                 maze.getNode(pos).gScore / float(maze.getPath().cost) * 360,
                 1.0f, 1.0f);

        sf::Color c;
        switch (state) {
            case EXPLORED:
                c = sf::Color(150, 200, 200, 40);
                break;
            case EVALUATED:
                colorGradient ? c = sf::Color(r * 255, g * 255, b * 255, 20)
                              : c = sf::Color(255, 255, 150, 40);
                break;
            case EXP_AND_EVAL:
                colorGradient ? c = sf::Color(r * 255, g * 255, b * 255, 70)
                              : c = sf::Color(255, 170, 170, 35);
                break;
            case DEFAULT:
            default:
                c = sf::Color(255, 255, 255, 5);
                break;
        }

        cell.setFillColor(c);
        target.draw(cell, states);

        auto hidden_c = sf::Color(255, 255, 255, 20);

        if (maze.isWall(pos, N))
            target.draw(line(0, 0, 1, 0), states);
        else if (virtualMaze.isWall(pos, N))
            target.draw(line(0, 0, 1, 0, hidden_c), states);

        if (maze.isWall(pos, S))
            target.draw(line(0, 1, 1, 1), states);
        else if (virtualMaze.isWall(pos, S))
            target.draw(line(0, 1, 1, 1, hidden_c), states);

        if (maze.isWall(pos, E))
            target.draw(line(1, 0, 1, 1), states);
        else if (virtualMaze.isWall(pos, E))
            target.draw(line(1, 0, 1, 1, hidden_c), states);

        if (maze.isWall(pos, W))
            target.draw(line(0, 0, 0, 1), states);
        else if (virtualMaze.isWall(pos, W))
            target.draw(line(0, 0, 0, 1, hidden_c), states);
    }

    Maze &maze, &virtualMaze;
    CellCoordinate pos;
};

class MazeDrawable : public sf::Transformable, public sf::Drawable {
  public:
    MazeDrawable(Maze &maze, Maze &virtualMaze)
        : maze(maze), virtualMaze(virtualMaze) {
    }

  private:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const {
        states.transform *= getTransform();

        for (int row = 0; row < Maze::CELL_ROWS; row++) {
            for (int col = 0; col < Maze::CELL_COLS; col++) {
                CellCoordinate pos(col, row);
                CellDrawable cell(maze, virtualMaze, pos);
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

    Maze &maze, &virtualMaze;
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
            } else if (keyPress(sf::Keyboard::G)) {
                colorGradient = !colorGradient;
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

        MazeDrawable entity(mouse.maze, mouse.virtualMaze);
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
