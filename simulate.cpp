#include <SFML/Graphics.hpp>

#include "Coordinate.h"
#include "Maze.h"

const int CELL_SIZE = 30;
const int WINDOW_WIDTH = 10 + Maze::CELL_COLS * CELL_SIZE;
const int WINDOW_HEIGHT = 10 + Maze::CELL_ROWS * CELL_SIZE;
const int MARGIN = 5;
const std::string WINDOW_TITLE = "Micromouse simulator";

class Simulator {
  public:
    Simulator(sf::RenderWindow &window) : window(window), maze() {
    }

    void drawBorder(void) {
        const sf::Vertex border[] = {
            sf::Vertex(sf::Vector2f(MARGIN, MARGIN)),
            sf::Vertex(sf::Vector2f(WINDOW_WIDTH - MARGIN, MARGIN)),
            sf::Vertex(
                sf::Vector2f(WINDOW_WIDTH - MARGIN, WINDOW_HEIGHT - MARGIN)),
            sf::Vertex(sf::Vector2f(MARGIN, WINDOW_HEIGHT - MARGIN)),
            sf::Vertex(sf::Vector2f(MARGIN, MARGIN)),
        };

        window.draw(border, 5, sf::LineStrip);
    }

    void draw(void) {
        window.clear(sf::Color::Black);

        for (int row = 0; row != Maze::NODE_ROWS; row++) {
            for (int col = 0; col != Maze::NODE_COLS; col++) {
                if (maze.isWall(NodeCoordinate(row, col))) {
                }
            }
        }

        drawBorder();

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
