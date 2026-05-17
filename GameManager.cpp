#include "GameManager.h"

#include <random>
#include <ctime>
#include <memory>

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
    hero("hero.png", 0)
{
    window.setFramerateLimit(60);

    map.randomizeBlocked(10);

    map.setCellBlocked(
        hero.getCurrentCellIndex(),
        false
    );

    spawnEnemies();
}

void GameManager::run() {
    while (window.isOpen()) {
        processEvents();
        update();
        render();
    }
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
                map.randomizeBlocked(10);

                map.setCellBlocked(
                    hero.getCurrentCellIndex(),
                    false
                );

                spawnEnemies();
            }

            if (keyPressed->code == sf::Keyboard::Key::M) {
                map.changeTheme();

                map.randomizeBlocked(10);

                map.setCellBlocked(
                    hero.getCurrentCellIndex(),
                    false
                );

                spawnEnemies();
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
                        clickedEnemy->takeDamage(1);
                        updateEnemies();
                    }
                }
                else {
                    hero.tryMoveTo(clickedCell, map);

                    if (hero.getCurrentCellIndex() != oldHeroCell) {
                        updateEnemies();
                    }
                }
            }
        }
    }
}

void GameManager::update() {
    if (!hero.isAlive()) {
        window.close();
    }
}

void GameManager::render() {
    window.clear(sf::Color::Black);

    map.render(window);

    for (auto& enemy : enemies) {
        enemy->render(window, map);
    }

    hero.render(window, map);

    window.display();
}

void GameManager::spawnEnemies() {
    enemies.clear();

    static std::mt19937 rng((unsigned)std::time(nullptr));

    const int enemyCount = 5;

    std::uniform_int_distribution<int> cellDist(
        0,
        map.getCellsCount() - 1
    );

    std::uniform_int_distribution<int> typeDist(0, 2);

    int placed = 0;

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

        int typeValue = typeDist(rng);

        if (typeValue == 0) {
            enemies.push_back(
                std::make_unique<Enemy>(
                    EnemyType::Zombie,
                    "zombie.png",
                    cellIndex
                )
            );
        }
        else if (typeValue == 1) {
            enemies.push_back(
                std::make_unique<Enemy>(
                    EnemyType::Skeleton,
                    "skeleton.png",
                    cellIndex
                )
            );
        }
        else {
            enemies.push_back(
                std::make_unique<Enemy>(
                    EnemyType::Demon,
                    "demon.png",
                    cellIndex
                )
            );
        }

        placed++;
    }
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

    return occupied;
}

void GameManager::updateEnemies() {
    for (auto& enemy : enemies) {
        if (!enemy->isAlive()) {
            continue;
        }

        // якщо ворог ¬∆≈ був б≥л€ геро€ до руху Ч в≥н атакуЇ
        if (enemy->isNearHero(
            hero.getCurrentCellIndex(),
            map
        )) {
            enemy->attackHero(hero);
            continue;
        }

        // якщо ворог не був б≥л€ геро€ Ч в≥н т≥льки рухаЇтьс€
        std::vector<int> occupiedCells =
            getOccupiedEnemyCells();

        enemy->updateAI(
            hero.getCurrentCellIndex(),
            map,
            occupiedCells
        );

        // ¬ј∆Ћ»¬ќ:
        // п≥сл€ руху ворог Ќ≈ атакуЇ одразу
    }
}