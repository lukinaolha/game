#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <ctime>
#include <iostream>
#include <stdexcept>
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

sf::Texture loadTextureTransparentBlack(const std::string& filename) {
    sf::Image image;

    if (!image.loadFromFile(filename)) {
        throw std::runtime_error("Cannot load file: " + filename);
    }

    image.createMaskFromColor(sf::Color::Black);

    sf::Texture texture;

    if (!texture.loadFromImage(image)) {
        throw std::runtime_error("Cannot create texture from: " + filename);
    }

    return texture;
}

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

void randomizeBlocked(std::vector<HexCell>& cells, int blockedCount) {
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

MapTheme getRandomTheme(const std::vector<MapTheme>& themes) {
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

int main() {
    try {
        sf::VideoMode desktop = sf::VideoMode::getDesktopMode();

        const unsigned int WINDOW_WIDTH = desktop.size.x;
        const unsigned int WINDOW_HEIGHT = desktop.size.y;

        sf::RenderWindow window(
            sf::VideoMode({
                WINDOW_WIDTH,
                WINDOW_HEIGHT
                }),
            "Turn-Based Strategy",
            sf::State::Fullscreen
        );

        window.setFramerateLimit(60);

        std::vector<MapTheme> themes = {
            { "stone.png", "lava.png" },
            { "grass.png", "water.png" },
            { "sand.png", "cactus.png" }
        };

        const float GAME_AREA_WIDTH = 850.0f;

        const float gameAreaCenterX =
            WINDOW_WIDTH / 2.0f;

        const float baseHexW = 33.0f;
        const float baseHexH = 25.0f;

        const float baseStepX = 52.0f;
        const int maxCount = 5;

        float baseMapWidth =
            (maxCount - 1) * baseStepX + baseHexW;

        float scale =
            (GAME_AREA_WIDTH * 0.90f) / baseMapWidth;

        const float hexW = baseHexW * scale;
        const float hexH = baseHexH * scale;

        const float fieldStartY = 80.0f;

        std::vector<HexCell> cells = buildField(
            gameAreaCenterX,
            fieldStartY,
            hexW,
            hexH
        );

        randomizeBlocked(cells, 10);

        MapTheme currentTheme =
            getRandomTheme(themes);

        sf::Texture floorTexture =
            loadTextureTransparentBlack(
                currentTheme.floorTextureFile
            );

        sf::Texture blockedTexture =
            loadTextureTransparentBlack(
                currentTheme.blockedTextureFile
            );

        sf::Sprite floorSprite(floorTexture);
        sf::Sprite blockedSprite(blockedTexture);

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

        while (window.isOpen()) {
            while (const std::optional event =
                window.pollEvent()) {

                if (event->is<sf::Event::Closed>()) {
                    window.close();
                }

                if (const auto* keyPressed =
                    event->getIf<sf::Event::KeyPressed>()) {

                    if (keyPressed->code ==
                        sf::Keyboard::Key::Escape) {

                        window.close();
                    }

                    if (keyPressed->code ==
                        sf::Keyboard::Key::R) {

                        randomizeBlocked(cells, 10);
                    }

                    if (keyPressed->code ==
                        sf::Keyboard::Key::M) {

                        currentTheme =
                            getRandomTheme(themes);

                        applyTheme(
                            currentTheme,
                            floorTexture,
                            blockedTexture,
                            floorSprite,
                            blockedSprite,
                            hexW,
                            hexH
                        );

                        randomizeBlocked(cells, 10);
                    }
                }
            }

            window.clear(sf::Color::Black);

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

            window.display();
        }
    }
    catch (const std::exception& e) {
        std::cout << e.what() << '\n';
    }

    return 0;
}