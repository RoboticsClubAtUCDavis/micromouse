#include <SFML/Graphics.hpp>

#include "Maze.h"

class Simulator {
  public:
    Simulator(void) : maze() {
    }
    void draw(sf::RenderWindow &window) {
    }

  private:
    Maze maze;
};

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Micromouse simulator");
    Simulator simulator;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);
        simulator.draw(window);
        window.display();
    }

    return 0;
}
