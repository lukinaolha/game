#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

#include "Map.h"

struct Hero {
    int cellIndex;
    sf::Texture texture;
    sf::Sprite sprite;

    Hero();
};

void setupHero(
    Hero& hero,
    const std::string& filename,
    float hexW,
    float hexH
);

void placeHeroOnCell(
    Hero& hero,
    const std::vector<HexCell>& cells
);

void moveHeroToFirstFreeCell(
    Hero& hero,
    const std::vector<HexCell>& cells
);