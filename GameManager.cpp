#include "GameManager.h"

#include <random>
#include <ctime>
#include <memory>
#include <iostream>

GameManager::GameManager()
    : window(
        sf::VideoMode({
            sf::VideoMode::getDesktopMode().size.x,
            sf::VideoMode::getDesktopMode().size.y
            }),
        "Turn-Based Strategy",
        sf::State::Fullscreen
    ),
    map(
        sf::VideoMode::getDesktopMode().size.x,
        sf::VideoMode::getDesktopMode().size.y
    ),
    hero("hero.png", 0),
    fontLoaded(false),
    currentLevel(1),
    maxLevel(15),
    backgroundColor(sf::Color::Black),
    gameAreaWidth(850.0f),
    gameAreaLeft(0.0f),
    stairsVisible(false),
    stairsCellIndex(-1),
    stairsSprite(stairsTexture),
    chestVisible(false),
    chestCellIndex(-1),
    chestSprite(chestTexture)
{
    window.setFramerateLimit(60);

    gameAreaLeft =
        (window.getSize().x - gameAreaWidth) / 2.0f;

    if (font.openFromFile("GILSANUB.ttf")) {
        fontLoaded = true;
    }
    else {
        std::cout << "Cannot load font file: arial.ttf" << std::endl;
    }

    if (!stairsTexture.loadFromFile("stairs.png")) {
        std::cout << "Cannot load stairs.png" << std::endl;
    }

    stairsSprite.setTexture(stairsTexture, true);

    stairsSprite.setOrigin({
        stairsTexture.getSize().x / 2.0f,
        stairsTexture.getSize().y / 2.0f
        });

    stairsSprite.setScale({
        map.getHexW() / stairsTexture.getSize().x,
        map.getHexH() / stairsTexture.getSize().y
        });

    if (!chestTexture.loadFromFile("chest.png")) {
        std::cout << "Cannot load chest.png" << std::endl;
    }

    chestSprite.setTexture(chestTexture, true);

    chestSprite.setOrigin({
        chestTexture.getSize().x / 2.0f,
        chestTexture.getSize().y / 2.0f
        });

    chestSprite.setScale({
        map.getHexW() / chestTexture.getSize().x,
        map.getHexH() / chestTexture.getSize().y
        });

    prepareLevel();
}

void GameManager::run() {
    while (window.isOpen()) {
        processEvents();
        update();
        render();
    }
}

void GameManager::prepareLevel() {
    stairsVisible = false;
    stairsCellIndex = -1;

    chestVisible = false;
    chestCellIndex = -1;

    updateBackgroundColor();

    map.setThemeByLevel(currentLevel);

    map.randomizeBlocked(10);

    map.setCellBlocked(
        hero.getCurrentCellIndex(),
        false
    );

    spawnEnemies();

    spawnChest();
}

void GameManager::goToNextLevel() {
    currentLevel++;

    if (currentLevel > maxLevel) {
        window.close();
        return;
    }

    prepareLevel();
}

void GameManager::updateBackgroundColor() {
    if (currentLevel <= 5) {
        // трава: 29494AFF
        backgroundColor = sf::Color(
            0x29,
            0x49,
            0x4A,
            0xFF
        );
    }
    else if (currentLevel <= 10) {
        // темніша пустеля
        backgroundColor = sf::Color(
            0xC4,
            0x7A,
            0x3A,
            0xFF
        );
    }
    else {
        // камінь: 322947FF
        backgroundColor = sf::Color(
            0x32,
            0x29,
            0x47,
            0xFF
        );
    }
}

void GameManager::renderGameAreaBackground() {
    sf::RectangleShape gameArea({
        gameAreaWidth,
        static_cast<float>(window.getSize().y)
        });

    gameArea.setPosition({
        gameAreaLeft,
        0.0f
        });

    gameArea.setFillColor(backgroundColor);

    window.draw(gameArea);
}

void GameManager::processEvents() {
    while (const std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }

        if (const auto* keyPressed =
            event->getIf<sf::Event::KeyPressed>()) {

            if (keyPressed->code == sf::Keyboard::Key::Escape) {
                window.close();
            }

            if (keyPressed->code == sf::Keyboard::Key::R) {
                prepareLevel();
            }

            if (keyPressed->code == sf::Keyboard::Key::N) {
                goToNextLevel();
            }
        }

        if (const auto* mousePressed =
            event->getIf<sf::Event::MouseButtonPressed>()) {

            if (mousePressed->button == sf::Mouse::Button::Left) {
                sf::Vector2f mousePos =
                    window.mapPixelToCoords({
                        mousePressed->position.x,
                        mousePressed->position.y
                        });

                int clickedCell =
                    map.getCellIndexByMouse(mousePos);

                if (clickedCell == -1) {
                    continue;
                }

                int oldHeroCell =
                    hero.getCurrentCellIndex();

                Enemy* clickedEnemy =
                    getEnemyOnCell(clickedCell);

                if (clickedEnemy != nullptr) {
                    if (map.areCellsNeighbours(
                        hero.getCurrentCellIndex(),
                        clickedCell
                    )) {
                        clickedEnemy->takeDamage(
                            hero.getDamage()
                        );

                        updateEnemies();
                    }
                }
                else {
                    hero.tryMoveTo(clickedCell, map);

                    if (hero.getCurrentCellIndex() != oldHeroCell) {
                        if (chestVisible &&
                            hero.getCurrentCellIndex() == chestCellIndex) {
                            collectChest();
                            updateEnemies();
                        }
                        else if (stairsVisible &&
                            hero.getCurrentCellIndex() == stairsCellIndex) {
                            goToNextLevel();
                        }
                        else {
                            updateEnemies();
                        }
                    }
                }
            }
        }
    }
}

void GameManager::update() {
    if (!hero.isAlive()) {
        window.close();
        return;
    }

    hero.updateTemporaryEffects();

    if (!stairsVisible && areAllEnemiesDead()) {
        spawnStairs();
    }
}

void GameManager::render() {
    // весь екран чорний
    window.clear(sf::Color::Black);

    // по центру кольоровий фон гри
    renderGameAreaBackground();

    map.render(window);

    renderChest();

    renderStairs();

    for (auto& enemy : enemies) {
        enemy->render(window, map);
    }

    hero.render(window, map);

    hero.renderStatusPanel(
        window,
        map,
        font,
        fontLoaded
    );

    window.display();
}

EnemyType GameManager::getEnemyTypeForCurrentLevel() const {
    if (currentLevel <= 5) {
        return EnemyType::Zombie;
    }

    if (currentLevel <= 10) {
        return EnemyType::Skeleton;
    }

    return EnemyType::Demon;
}

std::string GameManager::getEnemyTextureForCurrentLevel() const {
    if (currentLevel <= 5) {
        return "zombie.png";
    }

    if (currentLevel <= 10) {
        return "skeleton.png";
    }

    return "demon.png";
}

void GameManager::spawnEnemies() {
    enemies.clear();

    static std::mt19937 rng((unsigned)std::time(nullptr));

    const int enemyCount = 5;

    std::uniform_int_distribution<int> cellDist(
        0,
        map.getCellsCount() - 1
    );

    int placed = 0;

    EnemyType enemyType =
        getEnemyTypeForCurrentLevel();

    std::string enemyTexture =
        getEnemyTextureForCurrentLevel();

    while (placed < enemyCount) {
        int cellIndex = cellDist(rng);

        if (cellIndex == hero.getCurrentCellIndex()) {
            continue;
        }

        if (map.isCellBlocked(cellIndex)) {
            continue;
        }

        if (isEnemyOnCell(cellIndex)) {
            continue;
        }

        enemies.push_back(
            std::make_unique<Enemy>(
                enemyType,
                enemyTexture,
                cellIndex
            )
        );

        placed++;
    }
}

void GameManager::spawnChest() {
    static std::mt19937 rng((unsigned)std::time(nullptr));

    std::uniform_int_distribution<int> cellDist(
        0,
        map.getCellsCount() - 1
    );

    while (true) {
        int cellIndex = cellDist(rng);

        if (cellIndex == hero.getCurrentCellIndex()) {
            continue;
        }

        if (map.isCellBlocked(cellIndex)) {
            continue;
        }

        if (isEnemyOnCell(cellIndex)) {
            continue;
        }

        chestCellIndex = cellIndex;
        chestVisible = true;

        map.setCellBlocked(
            chestCellIndex,
            false
        );

        return;
    }
}

void GameManager::collectChest() {
    if (!chestVisible) {
        return;
    }

    chestVisible = false;
    chestCellIndex = -1;

    static std::mt19937 rng((unsigned)std::time(nullptr));

    std::uniform_int_distribution<int> rewardDist(0, 2);

    int reward = rewardDist(rng);

    if (reward == 0) {
        hero.healFull();
        std::cout << "Chest reward: full HP restored" << std::endl;
    }
    else if (reward == 1) {
        hero.activateAttackBoost();
        std::cout << "Chest reward: attack +1 for 60 seconds" << std::endl;
    }
    else {
        std::cout << "Chest reward: empty" << std::endl;
    }
}

void GameManager::renderChest() {
    if (!chestVisible || chestCellIndex == -1) {
        return;
    }

    const HexCell& cell =
        map.getCell(chestCellIndex);

    chestSprite.setPosition({
        cell.x,
        cell.y
        });

    window.draw(chestSprite);
}

void GameManager::spawnStairs() {
    static std::mt19937 rng((unsigned)std::time(nullptr));

    std::uniform_int_distribution<int> cellDist(
        0,
        map.getCellsCount() - 1
    );

    while (true) {
        int cellIndex = cellDist(rng);

        if (cellIndex == hero.getCurrentCellIndex()) {
            continue;
        }

        if (map.isCellBlocked(cellIndex)) {
            continue;
        }

        if (isEnemyOnCell(cellIndex)) {
            continue;
        }

        if (chestVisible && cellIndex == chestCellIndex) {
            continue;
        }

        stairsCellIndex = cellIndex;
        stairsVisible = true;

        map.setCellBlocked(
            stairsCellIndex,
            false
        );

        return;
    }
}

void GameManager::renderStairs() {
    if (!stairsVisible || stairsCellIndex == -1) {
        return;
    }

    const HexCell& cell =
        map.getCell(stairsCellIndex);

    stairsSprite.setPosition({
        cell.x,
        cell.y
        });

    window.draw(stairsSprite);
}

bool GameManager::areAllEnemiesDead() const {
    if (enemies.empty()) {
        return false;
    }

    for (const auto& enemy : enemies) {
        if (enemy->isAlive()) {
            return false;
        }
    }

    return true;
}

bool GameManager::isEnemyOnCell(int cellIndex) const {
    if (cellIndex < 0) {
        return false;
    }

    for (const auto& enemy : enemies) {
        if (enemy->isAlive() &&
            enemy->getCurrentCellIndex() == cellIndex) {
            return true;
        }
    }

    return false;
}

Enemy* GameManager::getEnemyOnCell(int cellIndex) {
    if (cellIndex < 0) {
        return nullptr;
    }

    for (auto& enemy : enemies) {
        if (enemy->isAlive() &&
            enemy->getCurrentCellIndex() == cellIndex) {
            return enemy.get();
        }
    }

    return nullptr;
}

std::vector<int> GameManager::getOccupiedEnemyCells() const {
    std::vector<int> occupied;

    for (const auto& enemy : enemies) {
        if (enemy->isAlive()) {
            occupied.push_back(enemy->getCurrentCellIndex());
        }
    }

    if (chestVisible && chestCellIndex != -1) {
        occupied.push_back(chestCellIndex);
    }

    return occupied;
}

void GameManager::updateEnemies() {
    for (auto& enemy : enemies) {
        if (!enemy->isAlive()) {
            continue;
        }

        if (enemy->isNearHero(
            hero.getCurrentCellIndex(),
            map
        )) {
            enemy->attackHero(hero);
            continue;
        }

        std::vector<int> occupiedCells =
            getOccupiedEnemyCells();

        enemy->updateAI(
            hero.getCurrentCellIndex(),
            map,
            occupiedCells
        );
    }
}