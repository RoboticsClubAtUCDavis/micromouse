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

#pragma once

#include "../neat/NetworkGenotype.h"
#include <SFML/Graphics.hpp>
#include <assert.h>
#include <vector>

class ImageEvolver {
  public:
    struct CandidateSolution {
        std::vector<sf::Vector2f> _positions;
    };

  private:
    CandidateSolution _best;

    float _bestFitness; // For most fit one in order to keep track of best
                        // across all generations

    std::vector<size_t> _bestConnectionIndices;

    std::vector<size_t> _activeNodeIndexToNodeIndex;
    std::vector<size_t> _nodeIndexToActiveNodeIndex;

    sf::Rect<float> _bounds;

  public:
    void train(const neat::NetworkGenotype &genotype, size_t populationSize,
               size_t numGenerations, std::mt19937 &generator);

    sf::Vector2f operator[](size_t index) const {
        return _best._positions[index];
    }

    size_t getNodeIndex(size_t activeNodeIndex) const {
        return _activeNodeIndexToNodeIndex[activeNodeIndex];
    }

    size_t getActiveNodeIndex(size_t nodeIndex) const {
        return _nodeIndexToActiveNodeIndex[nodeIndex];
    }

    size_t size() const {
        return _best._positions.size();
    }

    sf::Rect<float> getBounds() const {
        return _bounds;
    }
};

float distance(const sf::Vector2f &v1, const sf::Vector2f &v2);
float magnitude(const sf::Vector2f &v);

sf::Vector2f normalize(const sf::Vector2f &v);