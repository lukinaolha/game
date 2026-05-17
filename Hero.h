#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include "Map.h"

class Hero {
private:
    int currentCellIndex;

    int hp;
    int damage;
    bool alive;

    sf::Texture texture;
    sf::Sprite sprite;

public:
    Hero(
        const std::string& textureFile,
        int startCellIndex
    );

    void tryMoveTo(
        int targetCellIndex,
        const Map& map
    );

    void takeDamage(int amount);

    bool isAlive() const;

    int getCurrentCellIndex() const;

    void render(
        sf::RenderWindow& window,
        const Map& map
    );
};