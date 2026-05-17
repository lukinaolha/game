#pragma once

#include <SFML/Graphics.hpp>
#include <string>

#include "Map.h"

class Hero {
private:
    int currentCellIndex;

    int hp;
    int maxHp;

    int baseDamage;
    int damage;

    bool alive;

    bool attackBoostActive;
    sf::Clock attackBoostClock;
    float attackBoostDurationSeconds;

    sf::Texture texture;
    sf::Sprite sprite;

    sf::Texture fullHeartTexture;
    sf::Texture emptyHeartTexture;

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

    void healFull();

    void activateAttackBoost();

    void updateTemporaryEffects();

    bool isAlive() const;

    int getCurrentCellIndex() const;

    int getDamage() const;

    int getAttackBoostRemainingSeconds() const;

    void render(
        sf::RenderWindow& window,
        const Map& map
    );

    void renderStatusPanel(
        sf::RenderWindow& window,
        const Map& map,
        const sf::Font& font,
        bool fontLoaded
    );
};