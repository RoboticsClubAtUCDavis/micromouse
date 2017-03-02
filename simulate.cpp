#include <SFML/Graphics.hpp>

#include "Coordinate.h"
#include "Direction.h"
#include "Maze.h"

const int MARGIN = 5;
const std::string WINDOW_TITLE = "Micromouse simulator";

class Simulator : public sf::RenderWindow {
  public:
    Simulator(void)
        : maze(), sf::RenderWindow(sf::VideoMode(800, 600), WINDOW_TITLE) {
        calculateNodeSize();
    }


    void main_loop(void) {
        while (isOpen()) {
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

    sf::Vertex nodeVertex(NodeCoordinate c) {
        int x_p = MARGIN + c.x * node_size;
        int y_p = MARGIN + (Maze::NODE_ROWS - c.y) * node_size;
        return sf::Vertex(sf::Vector2f(x_p, y_p));
    }

    void drawBorder(void) {
        const sf::Vertex border[] = {
            nodeVertex(NodeCoordinate(0, 0)),
            nodeVertex(NodeCoordinate(Maze::NODE_COLS, 0)),
            nodeVertex(NodeCoordinate(Maze::NODE_COLS, Maze::NODE_ROWS)),
            nodeVertex(NodeCoordinate(0, Maze::NODE_ROWS)),
            nodeVertex(NodeCoordinate(0, 0)),
        };

        draw(border, 5, sf::LineStrip);
    }

    void drawLine(NodeCoordinate c1, NodeCoordinate c2) {
        sf::Vertex line[] = {nodeVertex(c1), nodeVertex(c2)};
        draw(line, 2, sf::Lines);
    }

    void drawCell(CellCoordinate pos) {
        if (maze.isWall(pos, N)) {
            drawLine(pos + N, pos + NE);
        }

        if (maze.isWall(pos, S)) {
            drawLine(pos, pos + E);
        }

        if (maze.isWall(pos, E)) {
            drawLine(pos + E, pos + NE);
        }

        if (maze.isWall(pos, W)) {
            drawLine(pos, pos + N);
        }
    }

    void render(void) {
        clear(sf::Color::Black);

        drawBorder();

        for (int row = 0; row < Maze::CELL_ROWS; row++) {
            for (int col = 0; col < Maze::CELL_COLS; col++) {
                drawCell(CellCoordinate(col, row));
            }
        }

        display();
    }

    Maze maze;
    int node_size;
};

int main() {
    Simulator simulator;
    simulator.main_loop();
    return 0;
}
