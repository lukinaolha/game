#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <string>

#include "Map.h"
#include "Hero.h"
#include "Enemy.h"

class GameRenderer {
private:
    static sf::Color getBackgroundColorForLevel(
        int currentLevel
    );

    static void renderGameAreaBackground(
        sf::RenderWindow& window,
        float gameAreaWidth,
        float gameAreaLeft,
        sf::Color backgroundColor
    );

    static void renderObjectOnCell(
        sf::RenderWindow& window,
        const Map& map,
        sf::Sprite& sprite,
        bool visible,
        int cellIndex
    );

    static void renderRewardPopup(
        sf::RenderWindow& window,
        const sf::Font& font,
        bool fontLoaded,
        bool popupVisible,
        const std::string& popupTitle,
        const std::string& popupText
    );

public:
    static void render(
        sf::RenderWindow& window,
        Map& map,
        Hero& hero,
        std::vector<std::unique_ptr<Enemy>>& enemies,

        sf::Sprite& chestSprite,
        bool chestVisible,
        int chestCellIndex,

        sf::Sprite& stairsSprite,
        bool stairsVisible,
        int stairsCellIndex,

        const sf::Font& font,
        bool fontLoaded,

        float gameAreaWidth,
        float gameAreaLeft,
        int currentLevel,

        bool rewardPopupVisible,
        const std::string& rewardPopupTitle,
        const std::string& rewardPopupText
    );
};