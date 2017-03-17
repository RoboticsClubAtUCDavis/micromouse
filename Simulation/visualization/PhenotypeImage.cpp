/*
        NEAT Visualizer
        Copyright (C) 2012-2014 Eric Laukien

        This software is provided 'as-is', without any express or implied
        warranty.  In no event will the authors be held liable for any damages
        arising from the use of this software.

        Permission is granted to anyone to use this software for any purpose,
        including commercial applications, and to alter it and redistribute it
        freely, subject to the following restrictions:

        1. The origin of this software must not be misrepresented; you must not
                claim that you wrote the original software. If you use this
   software in a product, an acknowledgment in the product documentation would
   be appreciated but is not required. 2. Altered source versions must be
   plainly marked as such, and must not be misrepresented as being the original
   software. 3. This notice may not be removed or altered from any source
   distribution.
*/

#include "PhenotypeImage.h"
#include "../neat/NetworkGenotype.h"
#include "ImageEvolver.h"
#include <iostream>
#include <list>
#include <memory>
#include <random>
#include <sstream>

PhenotypeImage::PhenotypeImage(unsigned int width, unsigned int height) {
    _image.create(width, height);
}

void PhenotypeImage::update(size_t population, size_t numGenerations,
                            sf::RenderWindow &window, sf::Rect<float> region,
                            const neat::NetworkGenotype &genotype,
                            std::mt19937 &generator) {
    // Genetic algorithm to find best configuration
    ImageEvolver e;

    e.train(genotype, population, numGenerations, generator);

    _image.setActive();

    sf::Vector2f sizeRatio(region.width / e.getBounds().width,
                           region.height / e.getBounds().height);

    sf::VertexArray lineArray(sf::Quads);

    const float nodeRadius = 10.0f;
    const float lineWidth = 3.0f;

    sf::Color inputColor(sf::Color::Blue);
    sf::Color outputColor(sf::Color::Red);

    const size_t numNodes = e.size();

    std::vector<sf::Vector2f> positions(numNodes);

    // Draw points
    for (size_t i = 0; i < numNodes; i++) {
        sf::Vector2f thisPos(
            (e[i].x - e.getBounds().left) * sizeRatio.x + region.left,
            _image.getSize().y -
                ((e[i].y - e.getBounds().top) * sizeRatio.y + region.top));

        positions[i] = thisPos;

        const neat::NetworkGenotype::NodeData &data(
            genotype.getNodeData(e.getNodeIndex(i)));

        for (std::list<std::shared_ptr<neat::ConnectionGene>>::const_iterator
                 it = data._connections.begin();
             it != data._connections.end(); it++) {
            // Add connection lines (quads)
            size_t connectionActiveIndex =
                e.getActiveNodeIndex((*it)->_outIndex);

            sf::Vector2f nodePos(
                (e[connectionActiveIndex].x - e.getBounds().left) *
                        sizeRatio.x +
                    region.left,
                _image.getSize().y -
                    ((e[connectionActiveIndex].y - e.getBounds().top) *
                         sizeRatio.y +
                     region.top));

            // Perpendicular vector
            sf::Vector2f perpendicular(thisPos.y - nodePos.y,
                                       nodePos.x - thisPos.x);

            // Set to line width
            perpendicular /= sqrtf(perpendicular.x * perpendicular.x +
                                   perpendicular.y * perpendicular.y) /
                             lineWidth;

            lineArray.append(sf::Vertex(thisPos - perpendicular, inputColor));
            lineArray.append(sf::Vertex(thisPos + perpendicular, inputColor));

            lineArray.append(sf::Vertex(nodePos + perpendicular, outputColor));
            lineArray.append(sf::Vertex(nodePos - perpendicular, outputColor));
        }
    }

    // Draw inputs and outputs
    _image.draw(lineArray);

    sf::CircleShape cs;

    cs.setFillColor(sf::Color::Black);

    cs.setRadius(nodeRadius);
    cs.setOrigin(nodeRadius, nodeRadius);

    // Draw nodes on top
    for (size_t i = 0; i < numNodes; i++) {
        cs.setPosition(positions[i]);

        if (e.getNodeIndex(i) < genotype.getNumInputs())
            cs.setFillColor(sf::Color::Green);
        else if (e.getNodeIndex(i) >=
                 genotype.getNumInputs() + genotype.getNumHidden())
            cs.setFillColor(sf::Color::Yellow);
        else
            cs.setFillColor(sf::Color::Black);

        _image.draw(cs);
    }

    _image.display();

    window.setActive();
}

void PhenotypeImage::updateShowStats(
    size_t population, size_t numGenerations, sf::RenderWindow &window,
    sf::Rect<float> region, const neat::NetworkGenotype &genotype,
    const std::string &networkName, const sf::Font &font, unsigned int textSize,
    std::mt19937 &generator) {
    sf::Text t;
    t.setFont(font);
    t.setCharacterSize(textSize);

    const float textBorderOffset = 3.0f;
    const float textLineSpacing =
        static_cast<float>(t.getCharacterSize()) + 2.0f;
    const float textBoxRimSize = 8.0f;
    const float totalTextHeight =
        textBoxRimSize + 2.0f * textBorderOffset + 4.0f * textLineSpacing;

    region.height -= totalTextHeight;

    // Genetic algorithm to find best configuration
    ImageEvolver e;

    e.train(genotype, population, numGenerations, generator);

    _image.setActive();

    sf::Vector2f sizeRatio(region.width / e.getBounds().width,
                           region.height / e.getBounds().height);

    sf::VertexArray lineArray(sf::Quads);

    const float nodeRadius = 6.0f;
    const float lineWidth = 1.5f;

    sf::Color inputColor(sf::Color::Blue);
    sf::Color outputColor(sf::Color::Red);

    const size_t numNodes = e.size();

    std::vector<sf::Vector2f> positions(numNodes);

    // Draw points
    for (size_t i = 0; i < numNodes; i++) {
        sf::Vector2f thisPos(
            (e[i].x - e.getBounds().left) * sizeRatio.x + region.left,
            _image.getSize().y -
                ((e[i].y - e.getBounds().top) * sizeRatio.y + region.top));

        positions[i] = thisPos;

        const neat::NetworkGenotype::NodeData &data(
            genotype.getNodeData(e.getNodeIndex(i)));

        for (std::list<std::shared_ptr<neat::ConnectionGene>>::const_iterator
                 it = data._connections.begin();
             it != data._connections.end(); it++) {
            // Add connection lines (quads)
            size_t connectionActiveIndex =
                e.getActiveNodeIndex((*it)->_outIndex);

            sf::Vector2f nodePos(
                (e[connectionActiveIndex].x - e.getBounds().left) *
                        sizeRatio.x +
                    region.left,
                _image.getSize().y -
                    ((e[connectionActiveIndex].y - e.getBounds().top) *
                         sizeRatio.y +
                     region.top));

            // Perpendicular vector
            sf::Vector2f perpendicular(thisPos.y - nodePos.y,
                                       nodePos.x - thisPos.x);

            // Set to line width
            perpendicular /= sqrtf(perpendicular.x * perpendicular.x +
                                   perpendicular.y * perpendicular.y) /
                             lineWidth;

            lineArray.append(sf::Vertex(thisPos - perpendicular, inputColor));
            lineArray.append(sf::Vertex(thisPos + perpendicular, inputColor));

            lineArray.append(sf::Vertex(nodePos + perpendicular, outputColor));
            lineArray.append(sf::Vertex(nodePos - perpendicular, outputColor));
        }
    }

    // Draw inputs and outputs
    _image.draw(lineArray);

    sf::CircleShape cs;

    cs.setFillColor(sf::Color::Black);

    cs.setRadius(nodeRadius);
    cs.setOrigin(nodeRadius, nodeRadius);

    // Draw nodes on top
    for (size_t i = 0; i < numNodes; i++) {
        cs.setPosition(positions[i]);

        if (e.getNodeIndex(i) < genotype.getNumInputs())
            cs.setFillColor(sf::Color::Green);
        else if (e.getNodeIndex(i) >=
                 genotype.getNumInputs() + genotype.getNumHidden())
            cs.setFillColor(sf::Color::Yellow);
        else
            cs.setFillColor(sf::Color::Black);

        _image.draw(cs);
    }

    // Create an underlying background for stats
    sf::RectangleShape back(sf::Vector2f(static_cast<float>(_image.getSize().x),
                                         static_cast<float>(totalTextHeight)));
    back.setFillColor(sf::Color(220, 220, 220));
    _image.draw(back);
    back.setFillColor(sf::Color::Black);
    back.setPosition(0.0f, totalTextHeight - textBoxRimSize);
    back.setSize(
        sf::Vector2f(static_cast<float>(_image.getSize().x), textBoxRimSize));
    _image.draw(back);

    // Draw stats
    std::stringstream ss;

    ss << "# Inputs: " << genotype.getNumInputs();

    t.setString(ss.str());

//    t.setColor(sf::Color::Black);

    t.setPosition(textBorderOffset, textBorderOffset);
    _image.draw(t);

    ss.str("");
    ss << "# Hidden: " << genotype.getNumHidden();
    t.setString(ss.str());

    t.setPosition(textBorderOffset, textBorderOffset + textLineSpacing);
    _image.draw(t);

    ss.str("");
    ss << "# Outputs: " << genotype.getNumOutputs();
    t.setString(ss.str());

    t.setPosition(textBorderOffset, textBorderOffset + 2.0f * textLineSpacing);
    _image.draw(t);

    ss.str("");
    ss << "# Connections: " << genotype.getConnectionSet().size();
    t.setString(ss.str());

    t.setPosition(textBorderOffset, textBorderOffset + 3.0f * textLineSpacing);
    _image.draw(t);

    // Draw title
    t.setString(networkName);
    t.setCharacterSize(textSize * 2);
    t.setOrigin(t.getLocalBounds().width, t.getLocalBounds().height / 2.0f);

    t.setPosition(_image.getSize().x - t.getLocalBounds().height,
                  totalTextHeight / 4.0f);

    _image.draw(t);

    _image.display();

    window.setActive();
}

void PhenotypeImage::saveToFile(const std::string &fileName) {
    // Check that it was created
    assert(_image.getSize().x != 0);

    sf::Image image(_image.getTexture().copyToImage());

    image.saveToFile(fileName);
}