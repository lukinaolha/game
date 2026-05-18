#include "Enemy.h"
#include "TextureUtils.h"

#include <random>
#include <ctime>

Enemy::Enemy(
    EnemyType enemyType,
    const std::string& textureFile,
    int startCellIndex
)
    : type(enemyType),
    currentCellIndex(startCellIndex),
    hp(1),
    damage(1),
    alive(true),
    frozen(false),
    moveCooldown(0),
    offsetX(0.0f),
    offsetY(-10.0f),
    scaleX(2.20f),
    scaleY(2.10f),
    sprite(texture)
{
    initByType(textureFile);
}

void Enemy::initByType(const std::string& textureFile) {
    if (type == EnemyType::Zombie) {
        hp = 1;
        damage = 1;
        moveCooldown = 0;

        scaleX = 3.50f;
        scaleY = 3.40f;

        offsetX = 0.0f;
        offsetY = +5.0f;
    }
    else if (type == EnemyType::Skeleton) {
        hp = 2;
        damage = 1;
        moveCooldown = 0;

        scaleX = 3.80f;
        scaleY = 3.20f;

        offsetX = 0.0f;
        offsetY = -5.0f;
    }
    else if (type == EnemyType::Demon) {
        hp = 3;
        damage = 2;
        moveCooldown = 0;

        scaleX = 4.00f;
        scaleY = 3.80f;

        offsetX = 0.0f;
        offsetY = -3.0f;
    }

    texture = loadTextureTransparentBlack(textureFile);

    sprite.setTexture(texture, true);

    sprite.setOrigin({
        texture.getSize().x / 2.0f,
        texture.getSize().y / 2.0f
        });

    sprite.setScale({
        scaleX,
        scaleY
        });
}

EnemyType Enemy::getType() const {
    return type;
}

int Enemy::getCurrentCellIndex() const {
    return currentCellIndex;
}

bool Enemy::isAlive() const {
    return alive;
}

void Enemy::freeze() {
    if (!alive) {
        return;
    }

    frozen = true;
}

bool Enemy::isFrozen() const {
    return frozen;
}

void Enemy::takeDamage(int amount) {
    if (!alive) {
        return;
    }

    hp -= amount;

    if (hp <= 0) {
        alive = false;
    }
}

bool Enemy::isNearHero(
    int heroCellIndex,
    const Map& map
) const {
    if (!alive) {
        return false;
    }

    return map.areCellsNeighbours(
        currentCellIndex,
        heroCellIndex
    );
}

bool Enemy::isCellOccupied(
    int cellIndex,
    const std::vector<int>& occupiedCells
) const {
    for (int occupied : occupiedCells) {
        if (occupied == cellIndex) {
            return true;
        }
    }

    return false;
}

int Enemy::findBestMoveToHero(
    int heroCellIndex,
    const Map& map,
    const std::vector<int>& occupiedCells
) const {
    std::vector<int> neighbours =
        map.getNeighbourCells(currentCellIndex);

    int bestCell = -1;
    float bestDistance = 999999.0f;

    for (int cellIndex : neighbours) {
        if (map.isCellBlocked(cellIndex)) {
            continue;
        }

        if (cellIndex == heroCellIndex) {
            continue;
        }

        if (isCellOccupied(cellIndex, occupiedCells)) {
            continue;
        }

        float distance =
            map.getDistanceBetweenCells(
                cellIndex,
                heroCellIndex
            );

        if (distance < bestDistance) {
            bestDistance = distance;
            bestCell = cellIndex;
        }
    }

    return bestCell;
}

int Enemy::findRandomMove(
    int heroCellIndex,
    const Map& map,
    const std::vector<int>& occupiedCells
) const {
    std::vector<int> neighbours =
        map.getNeighbourCells(currentCellIndex);

    std::vector<int> available;

    for (int cellIndex : neighbours) {
        if (map.isCellBlocked(cellIndex)) {
            continue;
        }

        if (cellIndex == heroCellIndex) {
            continue;
        }

        if (isCellOccupied(cellIndex, occupiedCells)) {
            continue;
        }

        available.push_back(cellIndex);
    }

    if (available.empty()) {
        return -1;
    }

    static std::mt19937 rng((unsigned)std::time(nullptr));

    std::uniform_int_distribution<int> dist(
        0,
        (int)available.size() - 1
    );

    return available[dist(rng)];
}

void Enemy::updateAI(
    int heroCellIndex,
    const Map& map,
    const std::vector<int>& occupiedCells
) {
    if (!alive) {
        return;
    }

    if (frozen) {
        frozen = false;
        return;
    }

    if (isNearHero(heroCellIndex, map)) {
        return;
    }

    int targetCell = -1;

    if (type == EnemyType::Skeleton) {
        targetCell = findBestMoveToHero(
            heroCellIndex,
            map,
            occupiedCells
        );
    }
    else if (type == EnemyType::Demon) {
        if (moveCooldown == 0) {
            targetCell = findBestMoveToHero(
                heroCellIndex,
                map,
                occupiedCells
            );

            moveCooldown = 1;
        }
        else {
            moveCooldown = 0;
            return;
        }
    }
    else if (type == EnemyType::Zombie) {
        static std::mt19937 rng((unsigned)std::time(nullptr));
        std::uniform_int_distribution<int> dist(0, 99);

        int chance = dist(rng);

        if (chance < 50) {
            return;
        }
        else if (chance < 75) {
            targetCell = findRandomMove(
                heroCellIndex,
                map,
                occupiedCells
            );
        }
        else {
            targetCell = findBestMoveToHero(
                heroCellIndex,
                map,
                occupiedCells
            );
        }
    }

    if (targetCell != -1) {
        currentCellIndex = targetCell;
    }
}

void Enemy::attackHero(Hero& hero) {
    if (!alive) {
        return;
    }

    if (frozen) {
        frozen = false;
        return;
    }

    hero.takeDamage(damage);
}

void Enemy::render(
    sf::RenderWindow& window,
    const Map& map
) {
    if (!alive) {
        return;
    }

    const HexCell& cell =
        map.getCell(currentCellIndex);

    sprite.setPosition({
        cell.x + offsetX,
        cell.y + offsetY
        });

    window.draw(sprite);
}