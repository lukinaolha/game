#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <string>

#include "Map.h"
#include "Hero.h"
#include "Enemy.h"

class GameManager {
private:
    sf::RenderWindow window;

    Map map;
    Hero hero;

    std::vector<std::unique_ptr<Enemy>> enemies;

    sf::Font font;
    bool fontLoaded;

    int currentLevel;
    const int maxLevel;

    sf::Color backgroundColor;

    // нове: центральна ігрова зона
    float gameAreaWidth;
    float gameAreaLeft;

    bool stairsVisible;
    int stairsCellIndex;

    sf::Texture stairsTexture;
    sf::Sprite stairsSprite;

    bool chestVisible;
    int chestCellIndex;

    sf::Texture chestTexture;
    sf::Sprite chestSprite;

private:
    void processEvents();
    void update();
    void render();

    void prepareLevel();
    void goToNextLevel();

    void updateBackgroundColor();
    void renderGameAreaBackground();

    void spawnEnemies();
    void updateEnemies();

    void spawnStairs();
    void renderStairs();

    void spawnChest();
    void renderChest();
    void collectChest();

    bool areAllEnemiesDead() const;

    EnemyType getEnemyTypeForCurrentLevel() const;
    std::string getEnemyTextureForCurrentLevel() const;

    bool isEnemyOnCell(int cellIndex) const;
    Enemy* getEnemyOnCell(int cellIndex);

    std::vector<int> getOccupiedEnemyCells() const;

public:
    GameManager();

    void run();
};