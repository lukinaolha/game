#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

#include "Map.h"
#include "Hero.h"

enum class EnemyType {
    Zombie,
    Skeleton,
    Demon
};

class Enemy {
private:
    EnemyType type;

    int currentCellIndex;
    int hp;
    int damage;
    bool alive;
    bool frozen;

    int moveCooldown;

    float offsetX;
    float offsetY;
    float scaleX;
    float scaleY;

    sf::Texture texture;
    sf::Sprite sprite;

private:
    void initByType(const std::string& textureFile);

    int findBestMoveToHero(
        int heroCellIndex,
        const Map& map,
        const std::vector<int>& occupiedCells
    ) const;

    int findRandomMove(
        int heroCellIndex,
        const Map& map,
        const std::vector<int>& occupiedCells
    ) const;

    bool isCellOccupied(
        int cellIndex,
        const std::vector<int>& occupiedCells
    ) const;

public:
    Enemy(
        EnemyType enemyType,
        const std::string& textureFile,
        int startCellIndex
    );

    EnemyType getType() const;

    int getCurrentCellIndex() const;

    bool isAlive() const;

    void takeDamage(int amount);

    void freeze();
    bool isFrozen() const;

    bool isNearHero(
        int heroCellIndex,
        const Map& map
    ) const;

    void updateAI(
        int heroCellIndex,
        const Map& map,
        const std::vector<int>& occupiedCells
    );

    void attackHero(Hero& hero);

    void render(
        sf::RenderWindow& window,
        const Map& map
    );
};