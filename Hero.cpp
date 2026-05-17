#include "Hero.h"
#include "TextureUtils.h"

Hero::Hero(
    const std::string& textureFile,
    int startCellIndex
)
    : currentCellIndex(startCellIndex),
    hp(5),
    damage(1),
    alive(true),
    sprite(texture)
{
    texture = loadTextureTransparentBlack(textureFile);

    sprite.setTexture(texture, true);

    sprite.setOrigin({
        texture.getSize().x / 2.0f,
        texture.getSize().y / 2.0f
        });

    sprite.setScale({
        2.50f,
        2.30f
        });
}

void Hero::tryMoveTo(
    int targetCellIndex,
    const Map& map
) {
    if (!alive) {
        return;
    }

    if (targetCellIndex == -1) {
        return;
    }

    if (map.isCellBlocked(targetCellIndex)) {
        return;
    }

    if (!map.areCellsNeighbours(
        currentCellIndex,
        targetCellIndex
    )) {
        return;
    }

    currentCellIndex = targetCellIndex;
}

void Hero::takeDamage(int amount) {
    if (!alive) {
        return;
    }

    hp -= amount;

    if (hp <= 0) {
        alive = false;
    }
}

bool Hero::isAlive() const {
    return alive;
}

int Hero::getCurrentCellIndex() const {
    return currentCellIndex;
}

void Hero::render(
    sf::RenderWindow& window,
    const Map& map
) {
    if (!alive) {
        return;
    }

    const HexCell& cell =
        map.getCell(currentCellIndex);

    sprite.setPosition({
        cell.x,
        cell.y - 10.0f
        });

    window.draw(sprite);
}