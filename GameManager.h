#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

#include "Map.h"
#include "Hero.h"
#include "Enemy.h"

class GameManager {
private:
    sf::RenderWindow window;

    Map map;
    Hero hero;

    std::vector<std::unique_ptr<Enemy>> enemies;

private:
    void processEvents();
    void update();
    void render();

    void spawnEnemies();
    void updateEnemies();

    bool isEnemyOnCell(int cellIndex) const;
    Enemy* getEnemyOnCell(int cellIndex);

    std::vector<int> getOccupiedEnemyCells() const;

public:
    GameManager();

    void run();
};