#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

struct HexCell {
    int row;
    int indexInRow;
    bool blocked;
    float x;
    float y;
};

struct MapTheme {
    std::string floorTextureFile;
    std::string blockedTextureFile;
};

class Map {
private:
    std::vector<HexCell> cells;
    std::vector<MapTheme> themes;
    MapTheme currentTheme;

    sf::Texture floorTexture;
    sf::Texture blockedTexture;

    sf::Sprite floorSprite;
    sf::Sprite blockedSprite;

    float hexW;
    float hexH;
    float stepX;
    float stepY;

    unsigned int windowWidth;
    unsigned int windowHeight;
    int currentThemeIndex;

private:
    void initThemes();
    void initMap();
    void initSprites();

    std::vector<int> getRowPattern();

    std::vector<HexCell> buildField(
        float centerX,
        float startY
    );

    MapTheme getRandomTheme();

    void setupSprite(
        sf::Sprite& sprite,
        const sf::Texture& texture
    );

    void applyTheme(const MapTheme& theme);

public:
    Map(
        unsigned int windowWidth,
        unsigned int windowHeight
    );

    void render(sf::RenderWindow& window);

    void randomizeBlocked(int blockedCount);

    void changeTheme();

    void setThemeByLevel(int level);

    int getCellIndexByMouse(sf::Vector2f mousePos) const;

    bool isCellBlocked(int index) const;

    void setCellBlocked(int index, bool value);

    bool areCellsNeighbours(
        int firstIndex,
        int secondIndex
    ) const;

    std::vector<int> getNeighbourCells(int index) const;

    float getDistanceBetweenCells(
        int firstIndex,
        int secondIndex
    ) const;

    const HexCell& getCell(int index) const;

    int getCellsCount() const;

    int getCurrentThemeIndex() const;

    float getHexW() const;

    float getHexH() const;
};