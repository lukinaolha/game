#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

struct HexCell {
    int row;
    int indexInRow;
    bool blocked;
    float x;
    float y;
};

struct MapTheme {
    std::string floorTextureFile;
    std::string blockedTextureFile;
};

std::vector<int> getRowPattern();

std::vector<HexCell> buildField(
    float centerX,
    float startY,
    float hexW,
    float hexH
);

void randomizeBlocked(
    std::vector<HexCell>& cells,
    int blockedCount
);

MapTheme getRandomTheme(
    const std::vector<MapTheme>& themes
);

void setupSprite(
    sf::Sprite& sprite,
    const sf::Texture& texture,
    float hexW,
    float hexH
);

void applyTheme(
    const MapTheme& theme,
    sf::Texture& floorTexture,
    sf::Texture& blockedTexture,
    sf::Sprite& floorSprite,
    sf::Sprite& blockedSprite,
    float hexW,
    float hexH
);
