#include <SFML/Graphics.hpp>

#include "Coordinate.h"
#include "Direction.h"
#include "Maze.h"
#include "Path.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <stdexcept>

const int MARGIN = 5;
const std::string WINDOW_TITLE = "Micromouse simulator";

class MazeDrawable : public sf::Transformable, public sf::Drawable {
  public:
    MazeDrawable(Maze &maze) : maze(maze) {
        node_size = 1. / std::max<float>(Maze::NODE_COLS, Maze::NODE_ROWS);
    }

  private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
        states.transform *= getTransform();

        for (int row = 0; row < Maze::CELL_ROWS; row++) {
            for (int col = 0; col < Maze::CELL_COLS; col++) {
                drawCell(target, states, CellCoordinate(col, row));
            }
        }

        drawPath(target, states, maze.getPath());
    }

    sf::Vector2f nodeVector(NodeCoordinate c) const {
        float x_p = c.x * node_size;
        float y_p = (Maze::NODE_ROWS - c.y) * node_size;
        return sf::Vector2f(x_p, y_p);
    }

    sf::Vertex nodeVertex(NodeCoordinate c, sf::Color color) const {
        return sf::Vertex(nodeVector(c), color);
    }

    void drawLine(sf::RenderTarget& target, sf::RenderStates states, NodeCoordinate c1, NodeCoordinate c2,
                  sf::Color color = sf::Color::White) const {
        sf::Vertex line[] = {nodeVertex(c1, color), nodeVertex(c2, color)};
        target.draw(line, 2, sf::Lines, states);
    }

    void drawCell(sf::RenderTarget& target, sf::RenderStates states, CellCoordinate pos) const {
        NodeCoordinate node = pos;
        if (maze.isWall(pos, N)) {
            drawLine(target, states, node + NW, node + NE);
        }

        if (maze.isWall(pos, S)) {
            drawLine(target, states, node + SW, node + SE);
        }

        if (maze.isWall(pos, E)) {
            drawLine(target, states, node + SE, node + NE);
        }

        if (maze.isWall(pos, W)) {
            drawLine(target, states, node + SW, node + NW);
        }

        sf::RectangleShape cell(
            sf::Vector2f(node_size * 2 - .001, node_size * 2 - .001));
        cell.move(nodeVector(node + NW));
        cell.setFillColor(maze.getNode(node).evaluated
                              ? sf::Color(255, 255, 150, 20)
                              : sf::Color(255, 255, 255, 10));
        target.draw(cell, states);
    }

    void drawPath(sf::RenderTarget& target, sf::RenderStates states, const Path &path) const {
        NodeCoordinate node = path.start;
        for (auto &i : path) {
            NodeCoordinate nextNode = node + i;
            drawLine(target, states, node, nextNode, sf::Color::Green);
            node = nextNode;
        }
    }

    Maze& maze;
    float node_size;
};

class Simulator : public sf::RenderWindow {
  public:
    Simulator(void)
        : sf::RenderWindow(sf::VideoMode(800, 600), WINDOW_TITLE), maze() {
        calculateNodeSize();
        try {
            maze = Maze::fromFile("test.maze");
        } catch (const std::exception &e) {
            std::cout << e.what();
        }
    }

    void main_loop(void) {

        while (isOpen()) {
            sf::sleep(sf::milliseconds(800));
            maze.findPath(CellCoordinate(rand() % Maze::CELL_COLS,
                                         rand() % Maze::CELL_ROWS),
                          CellCoordinate(rand() % Maze::CELL_COLS,
                                         rand() % Maze::CELL_ROWS),
                          N);

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

  protected:
    virtual void onResize(void) {
        sf::RenderWindow::onResize();
        calculateNodeSize();
    }

  private:
    void calculateNodeSize(void) {
        sf::Vector2u size = getSize();
        maze_size = std::min(size.x, size.y) - 2 * MARGIN;
    }


    void render(void) {
        clear(sf::Color::Black);
        MazeDrawable entity(maze);
        entity.setPosition(MARGIN, MARGIN);
        entity.setScale(maze_size, maze_size);
        draw(entity);
        display();
    }

    Maze maze;
    float maze_size;
};

int main() {
    srand(time(0));
    Simulator simulator;
    simulator.main_loop();
    return 0;
}
