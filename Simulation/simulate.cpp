#include <SFML/Graphics.hpp>

#include "../Micromouse/Coordinate.h"
#include "../Micromouse/Direction.h"
#include "../Micromouse/Maze.h"
#include "../Micromouse/Mouse.h"
#include "../Micromouse/Node.h"
#include "neat/Evolver.h"
#include "neat/NetworkPhenotype.h"
#include "neat/ParentSelectorProportional.h"
#include "visualization/PhenotypeImage.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <limits>
#include <memory>
#include <mutex>
#include <random>
#include <stdexcept>
#include <thread>
#include <time.h>

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
                // mouse.maze.findPath(CellCoordinate(rand() % Maze::CELL_COLS,
                //                                   rand() % Maze::CELL_ROWS),
                //                    CellCoordinate(rand() % Maze::CELL_COLS,
                //                                   rand() % Maze::CELL_ROWS));
            } else if (keyPress(sf::Keyboard::Return)) {
                // mouse.maze.findPath(CellCoordinate(0, 0), CellCoordinate(7,
                // 7));
            } else if (keyPress(sf::Keyboard::Space)) {
                // mouse.maze.generate();
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

float fitnessXOR(neat::NetworkPhenotype &phenotype);
void showOutputXOR(neat::NetworkPhenotype &phenotype);
void fitnessUpdateXOR(neat::Evolver &evolver);

const int NUM_INPUTS = (Maze::NODE_ROWS - 2) * (Maze::NODE_COLS - 2) +
                       1 /*facing*/ + 2 /*coordinate*/ +
                       4 /* available directions*/;

const int NUM_OUTPUTS = 4 /*direction to go*/;

int main() {
    srand(time(0));
    Mouse mouse;

    std::thread simulation(startSimulation, &mouse);
    // mouse.runMaze();

    std::mt19937 generator(time(nullptr));

    neat::Evolver evolver;

    std::cout << "Evolving network..." << std::endl;

    evolver.initialize(2, 1,
                       std::shared_ptr<neat::ParentSelectorProportional>(
                           new neat::ParentSelectorProportional()),
                       generator);

    const size_t numGenerations = 100;

    for (size_t g = 0; g < numGenerations; g++) {
        fitnessUpdateXOR(evolver);

        evolver.epoch(generator);
    }

    // Update fitnesses for final generation
    fitnessUpdateXOR(evolver);

    std::cout << "Evolved network done." << std::endl;

    // Find fittest
    std::shared_ptr<neat::NetworkGenotype> fittest;
    auto maxFitness = std::numeric_limits<float>::min();

    for (size_t i = 0, populationSize = evolver.getPopulationSize();
         i < populationSize; i++) {
        float fitness = evolver._population[i]._fitness;

        if (fitness > maxFitness) {
            maxFitness = fitness;
            fittest = evolver._population[i]._genotype;
        }
    }

    neat::NetworkPhenotype bestPhenotype;
    bestPhenotype.create(*fittest);

    showOutputXOR(bestPhenotype);

    std::cout << "Best fitness: " << fitnessXOR(bestPhenotype) * 25.0f << "%"
              << std::endl;
    std::cout << "Num Connections: " << fittest->getConnectionSet().size()
              << std::endl;
    std::cout << "Num Hidden Units: " << fittest->getNumHidden() << std::endl;

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(800, 600), "XOR Test",
                            sf::Style::Default, settings);

    PhenotypeImage image(800, 600);
    sf::Rect<float> region(60.0f, 60.0f, 680.0f, 480.0f);

    sf::Font font;

    if (!font.loadFromFile("times.ttf"))
        abort();

    image.updateShowStats(100, 100, window, region, *fittest, "XOR Test", font,
                          20, generator);

   // std::cout << "Image done. Saving to xor_image.png." << std::endl;

   // image.saveToFile("xor_image.png");

    sf::Sprite s(image.getTexture());

    window.setFramerateLimit(60);

    sf::Event event;

    bool quit = false;

    do {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                quit = true;
        }

        window.clear(sf::Color::White);

        window.draw(s);

        window.display();
    } while (!quit);

    window.close();

    simulation.join();
    return 0;
}

float fitnessXOR(neat::NetworkPhenotype &phenotype) {
    float inputs[][2] = {
        {0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}};

    float outputs[] = {0.0f, 1.0f, 1.0f, 0.0f};

    float fitness = 0.0f;

    // Test all input sets
    for (size_t i = 0; i < 4; i++) {
        phenotype.resetOutputs();
        phenotype.getInput(0)._output = inputs[i][0];
        phenotype.getInput(1)._output = inputs[i][1];

        // Simulate for 2 steps
        for (size_t j = 0; j < 2; j++)
            phenotype.update();

        // Get output difference
        fitness +=
            powf(std::abs(outputs[i] - phenotype.getOutput(0)._output), 2.0f);
    }

    return 4.0f - fitness;
}

void fitnessUpdateXOR(neat::Evolver &evolver) {
    for (size_t i = 0, populationSize = evolver.getPopulationSize();
         i < populationSize; i++) {
        neat::NetworkPhenotype phenotype;

        assert(evolver._population[i]._genotype != nullptr);

        phenotype.create(*evolver._population[i]._genotype);

        float fitness = fitnessXOR(phenotype);

        evolver._population[i]._fitness = std::pow(fitness, 2.0f);
    }
}

void showOutputXOR(neat::NetworkPhenotype &phenotype) {
    float inputs[][2] = {
        {0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}};

    // Test all input sets
    for (size_t i = 0; i < 4; i++) {
        phenotype.resetOutputs();
        phenotype.getInput(0)._output = inputs[i][0];
        phenotype.getInput(1)._output = inputs[i][1];

        // Simulate for 2 steps
        for (size_t j = 0; j < 2; j++)
            phenotype.update();

        // Get output difference
        std::cout << "Output @ (" << inputs[i][0] << ", " << inputs[i][1]
                  << "): " << phenotype.getOutput(0)._output << std::endl;
    }
}