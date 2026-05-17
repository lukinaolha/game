#include "Hero.h"

#include <iostream>
#include <algorithm>

Hero::Hero()
    : cellIndex(0),
    sprite(texture) {
}

void setupHero(
    Hero& hero,
    const std::string& filename,
    float hexW,
    float hexH
) {
    if (!hero.texture.loadFromFile(filename)) {
        std::cout << "Cannot load hero texture: " << filename << std::endl;
        return;
    }

    hero.sprite.setTexture(hero.texture, true);

    hero.sprite.setOrigin({
        hero.texture.getSize().x / 2.0f,
        hero.texture.getSize().y / 2.0f
        });

    float scaleX = hexW * 0.90f / hero.texture.getSize().x;
    float scaleY = hexH * 1.00f / hero.texture.getSize().y;

    float scale = std::min(scaleX, scaleY);

    hero.sprite.setScale({
        scale,
        scale
        });
}

void placeHeroOnCell(
    Hero& hero,
    const std::vector<HexCell>& cells
) {
    if (hero.cellIndex < 0 || hero.cellIndex >= (int)cells.size()) {
        return;
    }

    const HexCell& cell = cells[hero.cellIndex];

    hero.sprite.setPosition({
        cell.x,
        cell.y - 10.0f
        });
}

void moveHeroToFirstFreeCell(
    Hero& hero,
    const std::vector<HexCell>& cells
) {
    for (int i = 0; i < (int)cells.size(); i++) {
        if (!cells[i].blocked) {
            hero.cellIndex = i;
            return;
        }
    }
}