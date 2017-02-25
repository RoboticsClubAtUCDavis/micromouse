#include <SFML/Graphics.hpp>

#include "Coordinate.h"
#include "Direction.h"
#include "Maze.h"

const int CELL_SIZE = 30;
const int MARGIN = 5;
const int WINDOW_WIDTH = MARGIN * 2 + Maze::CELL_COLS * CELL_SIZE;
const int WINDOW_HEIGHT = MARGIN * 2 + Maze::CELL_ROWS * CELL_SIZE;
const std::string WINDOW_TITLE = "Micromouse simulator";

class Simulator {
  public:
    Simulator(sf::RenderWindow &window) : window(window), maze() {
    }

    sf::Vertex cellVertex(int x, int y) {
        int x_p = MARGIN + (Maze::CELL_COLS - x) * CELL_SIZE;
        int y_p = MARGIN + (Maze::CELL_ROWS - y) * CELL_SIZE;
        printf("%d %d\n", x_p, y_p);
        return sf::Vertex(sf::Vector2f(x_p, y_p));
    }

    void drawBorder(void) {
        const sf::Vertex border[] = {
            cellVertex(0, 0),
            cellVertex(Maze::CELL_COLS, 0),
            cellVertex(Maze::CELL_COLS, Maze::CELL_ROWS),
            cellVertex(0, Maze::CELL_ROWS),
            cellVertex(0, 0),
        };

        window.draw(border, 5, sf::LineStrip);
    }

    void drawLine(int x1, int y1, int x2, int y2) {
        sf::Vertex line[] = {cellVertex(x1, y1), cellVertex(x2, y2)};

        window.draw(line, 2, sf::Lines);
    }

    void drawCell(int row, int col) {
        CellCoordinate pos(col, row);

        if (maze.isWall(pos, N)) {
            drawLine(row + 1, col, row + 1, col + 1);
        }

        if (maze.isWall(pos, S)) {
            drawLine(row, col, row, col + 1);
        }

        if (maze.isWall(pos, E)) {
            drawLine(row, col + 1, row + 1, col + 1);
        }

        if (maze.isWall(pos, W)) {
            drawLine(row, col, row + 1, col);
        }
    }

    void draw(void) {
        window.clear(sf::Color::Black);

        drawBorder();

        for (int row = 0; row < Maze::CELL_ROWS; row++) {
            for (int col = 0; col < Maze::CELL_COLS; col++) {
                drawCell(row, col);
            }
        }

        window.display();
    }

  private:
    Maze maze;
    sf::RenderWindow &window;
};

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),
                            WINDOW_TITLE,
                            sf::Style::Titlebar | sf::Style::Close);
    Simulator simulator(window);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        simulator.draw();
    }

    return 0;
}
