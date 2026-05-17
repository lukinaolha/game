#include "Map.h"
#include "TextureUtils.h"

#include <random>
#include <ctime>
#include <cmath>

Map::Map(
    unsigned int w,
    unsigned int h
)
    : floorSprite(floorTexture),
    blockedSprite(blockedTexture),
    hexW(0.0f),
    hexH(0.0f),
    stepX(0.0f),
    stepY(0.0f),
    windowWidth(w),
    windowHeight(h)
{
    initThemes();
    initMap();
    initSprites();
}

void Map::initThemes() {
    themes = {
        { "grass.png", "water.png" },
        { "sand.png", "cactus.png" },
        { "stone.png", "lava.png" }
    };

    currentTheme = themes[0];
}

void Map::initMap() {
    const float GAME_AREA_WIDTH = 850.0f;

    const float baseHexW = 33.0f;
    const float baseHexH = 25.0f;

    const float baseStepX = 52.0f;
    const int maxCount = 5;

    float baseMapWidth =
        (maxCount - 1) * baseStepX + baseHexW;

    float scale =
        (GAME_AREA_WIDTH * 0.90f) / baseMapWidth;

    hexW = baseHexW * scale;
    hexH = baseHexH * scale;

    stepX = 48.0f * (hexW / 33.0f);
    stepY = 12.0f * (hexH / 25.0f);

    float centerX = windowWidth / 2.0f;
    float startY = 80.0f;

    cells = buildField(centerX, startY);
}

void Map::initSprites() {
    floorTexture =
        loadTextureTransparentBlack(
            currentTheme.floorTextureFile
        );

    blockedTexture =
        loadTextureTransparentBlack(
            currentTheme.blockedTextureFile
        );

    setupSprite(floorSprite, floorTexture);
    setupSprite(blockedSprite, blockedTexture);
}

std::vector<int> Map::getRowPattern() {
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

std::vector<HexCell> Map::buildField(
    float centerX,
    float startY
) {
    std::vector<HexCell> result;
    std::vector<int> pattern = getRowPattern();

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

            result.push_back(cell);
        }
    }

    return result;
}

void Map::randomizeBlocked(int blockedCount) {
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

MapTheme Map::getRandomTheme() {
    static std::mt19937 rng((unsigned)std::time(nullptr));

    std::uniform_int_distribution<int> dist(
        0,
        (int)themes.size() - 1
    );

    return themes[dist(rng)];
}

void Map::setupSprite(
    sf::Sprite& sprite,
    const sf::Texture& texture
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

void Map::applyTheme(const MapTheme& theme) {
    floorTexture =
        loadTextureTransparentBlack(
            theme.floorTextureFile
        );

    blockedTexture =
        loadTextureTransparentBlack(
            theme.blockedTextureFile
        );

    setupSprite(floorSprite, floorTexture);
    setupSprite(blockedSprite, blockedTexture);
}

void Map::changeTheme() {
    currentTheme = getRandomTheme();
    applyTheme(currentTheme);
}

void Map::setThemeByLevel(int level) {
    if (level <= 5) {
        currentTheme = {
            "grass.png",
            "water.png"
        };
    }
    else if (level <= 10) {
        currentTheme = {
            "sand.png",
            "cactus.png"
        };
    }
    else {
        currentTheme = {
            "stone.png",
            "lava.png"
        };
    }

    applyTheme(currentTheme);
}

void Map::render(sf::RenderWindow& window) {
    for (const auto& cell : cells) {
        if (cell.blocked) {
            blockedSprite.setPosition({
                cell.x,
                cell.y
                });

            window.draw(blockedSprite);
        }
        else {
            floorSprite.setPosition({
                cell.x,
                cell.y
                });

            window.draw(floorSprite);
        }
    }
}

int Map::getCellIndexByMouse(sf::Vector2f mousePos) const {
    int closestIndex = -1;
    float closestDistance = 999999.0f;

    for (int i = 0; i < (int)cells.size(); i++) {
        float dx = mousePos.x - cells[i].x;
        float dy = mousePos.y - cells[i].y;

        float distance = std::sqrt(dx * dx + dy * dy);

        if (distance < closestDistance) {
            closestDistance = distance;
            closestIndex = i;
        }
    }

    if (closestDistance > hexW * 0.65f) {
        return -1;
    }

    return closestIndex;
}

bool Map::isCellBlocked(int index) const {
    if (index < 0 || index >= (int)cells.size()) {
        return true;
    }

    return cells[index].blocked;
}

void Map::setCellBlocked(int index, bool value) {
    if (index < 0 || index >= (int)cells.size()) {
        return;
    }

    cells[index].blocked = value;
}

bool Map::areCellsNeighbours(
    int firstIndex,
    int secondIndex
) const {
    if (firstIndex < 0 || secondIndex < 0) {
        return false;
    }

    if (firstIndex >= (int)cells.size() ||
        secondIndex >= (int)cells.size()) {
        return false;
    }

    if (firstIndex == secondIndex) {
        return false;
    }

    float distance =
        getDistanceBetweenCells(
            firstIndex,
            secondIndex
        );

    float neighbourDistance =
        std::sqrt(
            (stepX / 2.0f) * (stepX / 2.0f) +
            stepY * stepY
        );

    return distance <= neighbourDistance * 1.25f;
}

std::vector<int> Map::getNeighbourCells(int index) const {
    std::vector<int> result;

    if (index < 0 || index >= (int)cells.size()) {
        return result;
    }

    for (int i = 0; i < (int)cells.size(); i++) {
        if (areCellsNeighbours(index, i)) {
            result.push_back(i);
        }
    }

    return result;
}

float Map::getDistanceBetweenCells(
    int firstIndex,
    int secondIndex
) const {
    if (firstIndex < 0 || secondIndex < 0) {
        return 999999.0f;
    }

    if (firstIndex >= (int)cells.size() ||
        secondIndex >= (int)cells.size()) {
        return 999999.0f;
    }

    float dx =
        cells[firstIndex].x - cells[secondIndex].x;

    float dy =
        cells[firstIndex].y - cells[secondIndex].y;

    return std::sqrt(dx * dx + dy * dy);
}

const HexCell& Map::getCell(int index) const {
    return cells[index];
}

int Map::getCellsCount() const {
    return (int)cells.size();
}

float Map::getHexW() const {
    return hexW;
}

float Map::getHexH() const {
    return hexH;
}