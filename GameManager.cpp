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
    currentState(GameState::Menu),
    map(
        sf::VideoMode::getDesktopMode().size.x,
        sf::VideoMode::getDesktopMode().size.y
    ),
    hero("hero.png", 0),
    fontLoaded(false),
    currentLevel(1),
    maxLevel(15),
    gameAreaWidth(850.0f),
    gameAreaLeft(0.0f),
    stairsVisible(false),
    stairsCellIndex(-1),
    stairsSprite(stairsTexture),
    chestVisible(false),
    chestCellIndex(-1),
    chestSprite(chestTexture),
    rewardPopupVisible(false),
    rewardPopupTitle(""),
    rewardPopupText("")
{
    window.setFramerateLimit(60);

    GameRenderer::initMenu(
        window.getSize().x,
        window.getSize().y
    );

    gameAreaLeft =
        (window.getSize().x - gameAreaWidth) / 2.0f;

    if (font.openFromFile("GILSANUB.ttf")) {
        fontLoaded = true;
    }
    else {
        std::cout << "Cannot load font file: GILSANUB.ttf" << std::endl;
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

    closeRewardPopup();

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

void GameManager::processEvents() {
    while (const std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }

        if (const auto* keyPressed =
            event->getIf<sf::Event::KeyPressed>()) {

            if (currentState == GameState::Menu) {
                if (keyPressed->code == sf::Keyboard::Key::Escape) {
                    window.close();
                }

                continue;
            }

            if (rewardPopupVisible) {
                if (keyPressed->code == sf::Keyboard::Key::Enter ||
                    keyPressed->code == sf::Keyboard::Key::Space ||
                    keyPressed->code == sf::Keyboard::Key::Escape) {
                    closeRewardPopup();
                }

                continue;
            }

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

                if (currentState == GameState::Menu) {
                    if (GameRenderer::isStartButtonClicked(mousePos)) {
                        currentState = GameState::Playing;
                    }

                    if (GameRenderer::isExitButtonClicked(mousePos)) {
                        window.close();
                    }

                    continue;
                }

                if (rewardPopupVisible) {
                    closeRewardPopup();
                    continue;
                }

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

                        if (clickedEnemy->isAlive()) {
                            clickedEnemy->freeze();
                        }

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
    if (currentState == GameState::Menu) {
        return;
    }

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
    if (currentState == GameState::Menu) {
        GameRenderer::renderMenu(window);
        return;
    }

    GameRenderer::render(
        window,
        map,
        hero,
        enemies,

        chestSprite,
        chestVisible,
        chestCellIndex,

        stairsSprite,
        stairsVisible,
        stairsCellIndex,

        font,
        fontLoaded,

        gameAreaWidth,
        gameAreaLeft,
        currentLevel,

        rewardPopupVisible,
        rewardPopupTitle,
        rewardPopupText
    );
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

int GameManager::getEnemyCountForCurrentLevel() const {
    if (currentLevel <= 5) {
        return 5;
    }

    if (currentLevel <= 10) {
        return 4;
    }

    return 3;
}

void GameManager::spawnEnemies() {
    enemies.clear();

    static std::mt19937 rng((unsigned)std::time(nullptr));

    int enemyCount =
        getEnemyCountForCurrentLevel();

    EnemyType enemyType =
        getEnemyTypeForCurrentLevel();

    std::string textureFile =
        getEnemyTextureForCurrentLevel();

    std::uniform_int_distribution<int> dist(
        0,
        map.getCellsCount() - 1
    );

    int placed = 0;

    while (placed < enemyCount) {
        int cellIndex = dist(rng);

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
                textureFile,
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

        showRewardPopup(
            "Chest reward",
            "Your health was fully restored!"
        );

        std::cout << "Chest reward: full HP restored" << std::endl;
    }
    else if (reward == 1) {
        hero.activateAttackBoost();

        showRewardPopup(
            "Chest reward",
            "Attack increased by +1\nfor 60 seconds!"
        );

        std::cout << "Chest reward: attack +1 for 60 seconds" << std::endl;
    }
    else {
        showRewardPopup(
            "Chest reward",
            "The chest was empty..."
        );

        std::cout << "Chest reward: empty" << std::endl;
    }
}

void GameManager::showRewardPopup(
    const std::string& title,
    const std::string& text
) {
    rewardPopupVisible = true;
    rewardPopupTitle = title;
    rewardPopupText = text;
}

void GameManager::closeRewardPopup() {
    rewardPopupVisible = false;
    rewardPopupTitle = "";
    rewardPopupText = "";
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
            occupied.push_back(
                enemy->getCurrentCellIndex()
            );
        }
    }

    if (chestVisible && chestCellIndex != -1) {
        occupied.push_back(chestCellIndex);
    }

    if (stairsVisible && stairsCellIndex != -1) {
        occupied.push_back(stairsCellIndex);
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