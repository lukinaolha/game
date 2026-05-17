#include "Map.h"
#include "TextureUtils.h"

#include <random>
#include <ctime>

std::vector<int> getRowPattern() {
    return {
        1,
        2,
        3,
        4,
        5,
        4,
        5,
        4,
        5,
        4,
        5,
        4,
        5,
        4,
        5,
        4,
        5,
        4,
        3,
        2,
        1
    };
}

std::vector<HexCell> buildField(
    float centerX,
    float startY,
    float hexW,
    float hexH
) {
    std::vector<HexCell> cells;
    std::vector<int> pattern = getRowPattern();

    float stepX = 48.0f * (hexW / 33.0f);
    float stepY = 12.0f * (hexH / 25.0f);

    for (int row = 0; row < (int)pattern.size(); row++) {
        int count = pattern[row];

        float rowWidth = (count - 1) * stepX;
        float startX = centerX - rowWidth / 2.0f;

        for (int i = 0; i < count; i++) {
            HexCell cell;

            cell.row = row;
            cell.indexInRow = i;
            cell.blocked = false;

            cell.x = startX + i * stepX;
            cell.y = startY + row * stepY;

            cells.push_back(cell);
        }
    }

    return cells;
}

void randomizeBlocked(
    std::vector<HexCell>& cells,
    int blockedCount
) {
    for (auto& cell : cells) {
        cell.blocked = false;
    }

    if (blockedCount > (int)cells.size()) {
        blockedCount = (int)cells.size();
    }

    static std::mt19937 rng((unsigned)std::time(nullptr));

    std::uniform_int_distribution<int> dist(
        0,
        (int)cells.size() - 1
    );

    int placed = 0;

    while (placed < blockedCount) {
        int index = dist(rng);

        if (!cells[index].blocked) {
            cells[index].blocked = true;
            placed++;
        }
    }
}

MapTheme getRandomTheme(
    const std::vector<MapTheme>& themes
) {
    static std::mt19937 rng((unsigned)std::time(nullptr));

    std::uniform_int_distribution<int> dist(
        0,
        (int)themes.size() - 1
    );

    return themes[dist(rng)];
}

void setupSprite(
    sf::Sprite& sprite,
    const sf::Texture& texture,
    float hexW,
    float hexH
) {
    sprite.setTexture(texture, true);

    sprite.setOrigin({
        texture.getSize().x / 2.0f,
        texture.getSize().y / 2.0f
        });

    sprite.setScale({
        hexW / texture.getSize().x,
        hexH / texture.getSize().y
        });
}

void applyTheme(
    const MapTheme& theme,
    sf::Texture& floorTexture,
    sf::Texture& blockedTexture,
    sf::Sprite& floorSprite,
    sf::Sprite& blockedSprite,
    float hexW,
    float hexH
) {
    floorTexture = loadTextureTransparentBlack(
        theme.floorTextureFile
    );

    blockedTexture = loadTextureTransparentBlack(
        theme.blockedTextureFile
    );

    setupSprite(
        floorSprite,
        floorTexture,
        hexW,
        hexH
    );

    setupSprite(
        blockedSprite,
        blockedTexture,
        hexW,
        hexH
    );
}