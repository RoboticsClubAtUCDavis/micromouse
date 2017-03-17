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

#include "ImageEvolver.h"
#include <SFML/Graphics.hpp>

class PhenotypeImage {
  private:
    sf::RenderTexture _image;

  public:
    PhenotypeImage(unsigned int width, unsigned int height);

    void update(size_t population, size_t numGenerations,
                sf::RenderWindow &window, sf::Rect<float> region,
                const neat::NetworkGenotype &genotype, std::mt19937 &generator);
    void updateShowStats(size_t population, size_t numGenerations,
                         sf::RenderWindow &window, sf::Rect<float> region,
                         const neat::NetworkGenotype &genotype,
                         const std::string &networkName, const sf::Font &font,
                         unsigned int textSize, std::mt19937 &generator);

    const sf::Texture &getTexture() const {
        return _image.getTexture();
    }

    void saveToFile(const std::string &fileName);
};
