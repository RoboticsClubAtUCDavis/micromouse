#include <SFML/Graphics.hpp>

#include "Coordinate.h"
#include "Direction.h"
#include "Maze.h"

const int MARGIN = 5;
const std::string WINDOW_TITLE = "Micromouse simulator";

class Simulator : public sf::RenderWindow {
  public:
    Simulator(void)
        : maze(),
          sf::RenderWindow(sf::VideoMode(800, 600), WINDOW_TITLE) {
        calculateCellSize();
    }

    void calculateCellSize(void) {
        sf::Vector2u size = getSize();
        int x = (size.x - 2 * MARGIN) / Maze::CELL_COLS;
        int y = (size.y - 2 * MARGIN) / Maze::CELL_ROWS;
        cell_size = std::min(x, y);
    }

    virtual void onResize(void) {
        calculateCellSize();
    }

    sf::Vertex cellVertex(int x, int y) {
        int x_p = MARGIN + (Maze::CELL_COLS - x) * cell_size;
        int y_p = MARGIN + (Maze::CELL_ROWS - y) * cell_size;
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

        draw(border, 5, sf::LineStrip);
    }

    void drawLine(int x1, int y1, int x2, int y2) {
        sf::Vertex line[] = {cellVertex(x1, y1), cellVertex(x2, y2)};

        draw(line, 2, sf::Lines);
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

    void render(void) {
        clear(sf::Color::Black);

        drawBorder();

        for (int row = 0; row < Maze::CELL_ROWS; row++) {
            for (int col = 0; col < Maze::CELL_COLS; col++) {
                drawCell(row, col);
            }
        }

        display();
    }

    void main_loop(void) {
        while (isOpen()) {
            sf::Event event;
            while (pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    close();
            }

            render();
        }
    }

  private:
    Maze maze;
    int cell_size;
};

int main() {
    Simulator simulator;
    simulator.main_loop();
    return 0;
}
