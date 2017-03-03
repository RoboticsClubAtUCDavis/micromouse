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
        int x = (size.x - 2 * MARGIN) / Maze::NODE_COLS;
        int y = (size.y - 2 * MARGIN) / Maze::NODE_ROWS;
        node_size = std::min(x, y);
    }

    sf::Vertex nodeVertex(NodeCoordinate c, sf::Color color) {
        int x_p = MARGIN + c.x * node_size;
        int y_p = MARGIN + (Maze::NODE_ROWS - c.y) * node_size;
        return sf::Vertex(sf::Vector2f(x_p, y_p), color);
    }

    void drawLine(NodeCoordinate c1, NodeCoordinate c2,
                  sf::Color color = sf::Color::White) {
        sf::Vertex line[] = {nodeVertex(c1, color), nodeVertex(c2, color)};
        draw(line, 2, sf::Lines);
    }

    void drawCell(CellCoordinate pos) {
        NodeCoordinate node = pos;
        if (maze.isWall(pos, N)) {
            drawLine(node + NW, node + NE);
        }

        if (maze.isWall(pos, S)) {
            drawLine(node + SW, node + SE);
        }

        if (maze.isWall(pos, E)) {
            drawLine(node + SE, node + NE);
        }

        if (maze.isWall(pos, W)) {
            drawLine(node + SW, node + NW);
        }

        sf::RectangleShape cell(
            sf::Vector2f(node_size * 2 - 1, node_size * 2 - 1));
        cell.move(sf::Vector2f(MARGIN + (node.x - 1) * node_size,
                               MARGIN + (Maze::NODE_ROWS - (node.y + 1)) *
                                            node_size));
        cell.setFillColor(maze.getNode(node).evaluated
                              ? sf::Color::Color(255, 255, 150, 20)
                              : sf::Color::Color(255, 255, 255, 10));
        draw(cell);
    }

    void drawPath(const Path &path) {
        NodeCoordinate node = path.start;
        for (auto &i : path) {
            NodeCoordinate nextNode = node + i;
            drawLine(node, nextNode, sf::Color::Green);
            node = nextNode;
        }
    }

    void render(void) {
        clear(sf::Color::Black);

        for (int row = 0; row < Maze::CELL_ROWS; row++) {
            for (int col = 0; col < Maze::CELL_COLS; col++) {
                drawCell(CellCoordinate(col, row));
            }
        }

        drawPath(maze.getPath());

        display();
    }

    Maze maze;
    int node_size;
};

int main() {
    srand(time(0));
    Simulator simulator;
    simulator.main_loop();
    return 0;
}
