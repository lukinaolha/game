#include <SFML/Graphics.hpp>

#include <vector>
#include <iostream>
#include <stdexcept>

#include "Map.h"
#include "Hero.h"
#include "TextureUtils.h"

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

        Hero hero;

        setupHero(
            hero,
            "hero.png",
            hexW,
            hexH
        );

        moveHeroToFirstFreeCell(
            hero,
            cells
        );

        placeHeroOnCell(
            hero,
            cells
        );

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

                        if (cells[hero.cellIndex].blocked) {
                            moveHeroToFirstFreeCell(
                                hero,
                                cells
                            );
                        }
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

                        if (cells[hero.cellIndex].blocked) {
                            moveHeroToFirstFreeCell(
                                hero,
                                cells
                            );
                        }
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

            placeHeroOnCell(
                hero,
                cells
            );

            window.draw(hero.sprite);

            window.display();
        }
    }
    catch (const std::exception& e) {
        std::cout << e.what() << '\n';
    }

    return 0;
}