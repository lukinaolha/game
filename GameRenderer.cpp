#include "GameRenderer.h"

#include <iostream>
#include <cmath>

sf::Texture GameRenderer::menuBackgroundTexture;
sf::Sprite GameRenderer::menuBackgroundSprite(menuBackgroundTexture);

sf::Texture GameRenderer::startButtonTexture;
sf::Sprite GameRenderer::startButtonSprite(startButtonTexture);

sf::Texture GameRenderer::exitButtonTexture;
sf::Sprite GameRenderer::exitButtonSprite(exitButtonTexture);

sf::Texture GameRenderer::rulesButtonTexture;
sf::Sprite GameRenderer::rulesButtonSprite(rulesButtonTexture);

sf::Texture GameRenderer::zombieRulesTexture;
sf::Sprite GameRenderer::zombieRulesSprite(zombieRulesTexture);

sf::Texture GameRenderer::skeletonRulesTexture;
sf::Sprite GameRenderer::skeletonRulesSprite(skeletonRulesTexture);

sf::Texture GameRenderer::demonRulesTexture;
sf::Sprite GameRenderer::demonRulesSprite(demonRulesTexture);

sf::Texture GameRenderer::victoryPanelTexture;
sf::Sprite GameRenderer::victoryPanelSprite(victoryPanelTexture);

sf::Texture GameRenderer::victoryButtonTexture;
sf::Sprite GameRenderer::victoryButtonSprite(victoryButtonTexture);

sf::Texture GameRenderer::wastedButtonTexture;
sf::Sprite GameRenderer::wastedButtonSprite(wastedButtonTexture);

namespace RenderColors {
    const sf::Color GrassBackground(0, 0, 0, 255);
    const sf::Color DesertBackground(0, 0, 0, 255);
    const sf::Color StoneBackground(0, 0, 0, 255);

    const sf::Color PopupOverlay(0, 0, 0, 180);
    const sf::Color PopupBackground(25, 25, 25, 245);
    const sf::Color PopupOutline(220, 220, 220, 255);

    const sf::Color TextWhite(255, 255, 255, 255);

    const sf::Color OkButton(93, 0, 0, 255);
}

void GameRenderer::initMenu(
    unsigned int windowWidth,
    unsigned int windowHeight
) {
    if (!menuBackgroundTexture.loadFromFile("menu_background.png")) {
        std::cout << "Cannot load menu_background.png" << std::endl;
    }

    menuBackgroundSprite.setTexture(menuBackgroundTexture, true);

    float backgroundScaleX =
        static_cast<float>(windowWidth) /
        menuBackgroundTexture.getSize().x;

    float backgroundScaleY =
        static_cast<float>(windowHeight) /
        menuBackgroundTexture.getSize().y;

    menuBackgroundSprite.setScale({
        backgroundScaleX,
        backgroundScaleY
        });

    if (!startButtonTexture.loadFromFile("start.png")) {
        std::cout << "Cannot load start.png" << std::endl;
    }

    startButtonSprite.setTexture(startButtonTexture, true);

    startButtonSprite.setOrigin({
        startButtonTexture.getSize().x / 2.0f,
        startButtonTexture.getSize().y / 2.0f
        });

    if (!exitButtonTexture.loadFromFile("exit.png")) {
        std::cout << "Cannot load exit.png" << std::endl;
    }

    exitButtonSprite.setTexture(exitButtonTexture, true);

    exitButtonSprite.setOrigin({
        exitButtonTexture.getSize().x / 2.0f,
        exitButtonTexture.getSize().y / 2.0f
        });

    if (!rulesButtonTexture.loadFromFile("rules.png")) {
        std::cout << "Cannot load rules.png" << std::endl;
    }

    rulesButtonSprite.setTexture(rulesButtonTexture, true);

    rulesButtonSprite.setOrigin({
        rulesButtonTexture.getSize().x / 2.0f,
        rulesButtonTexture.getSize().y / 2.0f
        });

    if (!zombieRulesTexture.loadFromFile("zombie.png")) {
        std::cout << "Cannot load zombie.png" << std::endl;
    }

    zombieRulesSprite.setTexture(zombieRulesTexture, true);
    zombieRulesSprite.setOrigin({
        zombieRulesTexture.getSize().x / 2.0f,
        zombieRulesTexture.getSize().y / 2.0f
        });

    if (!skeletonRulesTexture.loadFromFile("skeleton.png")) {
        std::cout << "Cannot load skeleton.png" << std::endl;
    }

    skeletonRulesSprite.setTexture(skeletonRulesTexture, true);
    skeletonRulesSprite.setOrigin({
        skeletonRulesTexture.getSize().x / 2.0f,
        skeletonRulesTexture.getSize().y / 2.0f
        });

    if (!demonRulesTexture.loadFromFile("demon.png")) {
        std::cout << "Cannot load demon.png" << std::endl;
    }

    demonRulesSprite.setTexture(demonRulesTexture, true);
    demonRulesSprite.setOrigin({
        demonRulesTexture.getSize().x / 2.0f,
        demonRulesTexture.getSize().y / 2.0f
        });

    float mainButtonScale = 6.0f;
    float rulesButtonScale = 4.0f;

    startButtonSprite.setScale({
        mainButtonScale,
        mainButtonScale
        });

    exitButtonSprite.setScale({
        mainButtonScale,
        mainButtonScale
        });

    rulesButtonSprite.setScale({
        rulesButtonScale,
        rulesButtonScale
        });

    float centerX =
        static_cast<float>(windowWidth) / 2.0f;

    startButtonSprite.setPosition({
        centerX,
        static_cast<float>(windowHeight) * 0.62f
        });

    exitButtonSprite.setPosition({
        centerX,
        static_cast<float>(windowHeight) * 0.76f
        });

    rulesButtonSprite.setPosition({
        static_cast<float>(windowWidth) - 70.0f,
        static_cast<float>(windowHeight) - 55.0f
        });
}

void GameRenderer::updateMenuButtonsHover(
    sf::Vector2f mousePos
) {
    bool startHovered =
        startButtonSprite
        .getGlobalBounds()
        .contains(mousePos);

    bool exitHovered =
        exitButtonSprite
        .getGlobalBounds()
        .contains(mousePos);

    bool rulesHovered =
        rulesButtonSprite
        .getGlobalBounds()
        .contains(mousePos);

    startButtonSprite.setColor(
        startHovered
        ? sf::Color(126, 126, 143, 255)
        : sf::Color::White
    );

    exitButtonSprite.setColor(
        exitHovered
        ? sf::Color(126, 126, 143, 255)
        : sf::Color::White
    );

    rulesButtonSprite.setColor(
        rulesHovered
        ? sf::Color(126, 126, 143, 255)
        : sf::Color::White
    );
}

void GameRenderer::renderMenu(
    sf::RenderWindow& window
) {
    window.clear(sf::Color::Black);

    window.draw(menuBackgroundSprite);
    window.draw(startButtonSprite);
    window.draw(exitButtonSprite);
    window.draw(rulesButtonSprite);

    window.display();
}

void GameRenderer::initVictory(
    unsigned int windowWidth,
    unsigned int windowHeight
) {
    if (!victoryPanelTexture.loadFromFile("panel.png")) {
        std::cout << "Cannot load panel.png" << std::endl;
    }

    victoryPanelSprite.setTexture(victoryPanelTexture, true);

    float panelScaleX =
        static_cast<float>(windowWidth) /
        victoryPanelTexture.getSize().x;

    float panelScaleY =
        static_cast<float>(windowHeight) /
        victoryPanelTexture.getSize().y;

    victoryPanelSprite.setScale({
        panelScaleX,
        panelScaleY
        });

    if (!victoryButtonTexture.loadFromFile("victory.png")) {
        std::cout << "Cannot load victory.png" << std::endl;
    }

    victoryButtonSprite.setTexture(victoryButtonTexture, true);

    float buttonScale = 15.0f;

    victoryButtonSprite.setScale({
        buttonScale,
        buttonScale
        });

    victoryButtonSprite.setOrigin({
        victoryButtonTexture.getSize().x / 2.0f,
        victoryButtonTexture.getSize().y / 2.0f
        });

    float centerX =
        static_cast<float>(windowWidth) / 2.0f;

    float centerY =
        static_cast<float>(windowHeight) * 0.10f +
        victoryButtonTexture.getSize().y * buttonScale / 2.0f;

    victoryButtonSprite.setPosition({
        centerX,
        centerY
        });
}

void GameRenderer::renderVictory(
    sf::RenderWindow& window
) {
    sf::Vector2f mousePos =
        window.mapPixelToCoords(
            sf::Mouse::getPosition(window)
        );

    updateVictoryButtonHover(mousePos);

    window.clear(sf::Color::Black);

    window.draw(victoryPanelSprite);
    window.draw(victoryButtonSprite);

    window.display();
}

void GameRenderer::initGameOver(
    unsigned int windowWidth,
    unsigned int windowHeight
) {
    if (!wastedButtonTexture.loadFromFile("wasted.png")) {
        std::cout << "Cannot load wasted.png" << std::endl;
    }

    wastedButtonSprite.setTexture(wastedButtonTexture, true);

    float buttonScale = 20.0f;

    wastedButtonSprite.setScale({
        buttonScale,
        buttonScale
        });

    wastedButtonSprite.setOrigin({
        wastedButtonTexture.getSize().x / 2.0f,
        wastedButtonTexture.getSize().y / 2.0f
        });

    float centerX =
        static_cast<float>(windowWidth) / 2.0f;

    float centerY =
        static_cast<float>(windowHeight) * 0.35f +
        wastedButtonTexture.getSize().y * buttonScale / 2.0f;

    wastedButtonSprite.setPosition({
        centerX,
        centerY
        });
}

void GameRenderer::renderGameOver(
    sf::RenderWindow& window
) {
    sf::Vector2f mousePos =
        window.mapPixelToCoords(
            sf::Mouse::getPosition(window)
        );

    updateGameOverButtonHover(mousePos);

    window.clear(sf::Color::Black);

    window.draw(wastedButtonSprite);

    window.display();
}

void GameRenderer::applyButtonHoverEffect(
    sf::Sprite& buttonSprite,
    float normalScale,
    sf::Vector2f mousePos
) {
    bool hovered =
        buttonSprite
        .getGlobalBounds()
        .contains(mousePos);

    if (hovered) {
        buttonSprite.setScale({
            normalScale * 1.08f,
            normalScale * 1.08f
            });
    }
    else {
        buttonSprite.setScale({
            normalScale,
            normalScale
            });
    }
}

void GameRenderer::updateVictoryButtonHover(
    sf::Vector2f mousePos
) {
    applyButtonHoverEffect(
        victoryButtonSprite,
        15.0f,
        mousePos
    );
}

void GameRenderer::updateGameOverButtonHover(
    sf::Vector2f mousePos
) {
    applyButtonHoverEffect(
        wastedButtonSprite,
        20.0f,
        mousePos
    );
}

void GameRenderer::renderRulesScreen(
    sf::RenderWindow& window,
    const sf::Font& font,
    bool fontLoaded
) {
    window.clear(sf::Color::Black);

    window.draw(menuBackgroundSprite);

    sf::RectangleShape overlay({
        static_cast<float>(window.getSize().x),
        static_cast<float>(window.getSize().y)
        });

    overlay.setPosition({ 0.0f, 0.0f });
    overlay.setFillColor(sf::Color(0, 0, 0, 190));
    window.draw(overlay);

    float panelWidth = 1120.0f;
    float panelHeight = 720.0f;

    float centerX = static_cast<float>(window.getSize().x) / 2.0f;
    float centerY = static_cast<float>(window.getSize().y) / 2.0f;

    sf::RectangleShape panel({ panelWidth, panelHeight });
    panel.setOrigin({ panelWidth / 2.0f, panelHeight / 2.0f });
    panel.setPosition({ centerX, centerY });
    panel.setFillColor(sf::Color(35, 30, 50, 245));
    panel.setOutlineColor(sf::Color(200, 200, 220));
    panel.setOutlineThickness(3.0f);

    window.draw(panel);

    if (!fontLoaded) {
        window.display();
        return;
    }

    sf::Text title(font);
    title.setString("Game rules");
    title.setCharacterSize(36);
    title.setFillColor(sf::Color::White);

    sf::FloatRect titleBounds = title.getLocalBounds();
    title.setOrigin({
        titleBounds.position.x + titleBounds.size.x / 2.0f,
        titleBounds.position.y + titleBounds.size.y / 2.0f
        });

    title.setPosition({
        centerX,
        centerY - panelHeight / 2.0f + 55.0f
        });

    window.draw(title);

    sf::Text leftText(font);
    leftText.setCharacterSize(21);
    leftText.setFillColor(sf::Color::White);
    leftText.setLineSpacing(1.08f);

    leftText.setString(
        "Goal:\n"
        "Defeat all enemies on the map\n"
        "and reach the stairs.\n\n"

        "Levels:\n"
        "The game has 15 levels.\n\n"

        "1-5:\n"
        "Grass map, zombies.\n\n"

        "6-10:\n"
        "Desert map, skeletons.\n\n"

        "11-15:\n"
        "Stone map, demons."
    );

    leftText.setPosition({
        centerX - 500.0f,
        centerY - 245.0f
        });

    window.draw(leftText);

    sf::Text controlsText(font);
    controlsText.setCharacterSize(20);
    controlsText.setFillColor(sf::Color::White);
    controlsText.setLineSpacing(1.08f);

    controlsText.setString(
        "Controls:\n"
        "Click a neighboring hex to move.\n"
        "Click a neighboring enemy to attack."
    );

    controlsText.setPosition({
        centerX - 500.0f,
        centerY + 210.0f
        });

    window.draw(controlsText);

    static sf::Clock pulseClock;

    float pulseTime = pulseClock.getElapsedTime().asSeconds();
    float pulseFactor = 1.0f + 0.06f * std::sin(pulseTime * 3.2f);

    float baseEnemyScale = 4.8f;
    float enemyScale = baseEnemyScale * pulseFactor;

    float enemyIconX = centerX + 25.0f;
    float enemyTextX = centerX + 95.0f;

    float zombieY = centerY - 185.0f;
    float skeletonY = centerY - 65.0f;
    float demonY = centerY + 55.0f;

    sf::Text enemiesTitle(font);
    enemiesTitle.setCharacterSize(21);
    enemiesTitle.setFillColor(sf::Color::White);
    enemiesTitle.setString("Enemies:");
    enemiesTitle.setPosition({
        enemyTextX,
        centerY - 245.0f
        });
    window.draw(enemiesTitle);

    sf::Sprite zombieSprite = zombieRulesSprite;
    zombieSprite.setScale({ enemyScale, enemyScale });
    zombieSprite.setPosition({ enemyIconX, zombieY + 35.0f });
    window.draw(zombieSprite);

    sf::Text zombieText(font);
    zombieText.setCharacterSize(21);
    zombieText.setFillColor(sf::Color::White);
    zombieText.setLineSpacing(1.08f);
    zombieText.setString(
        "Zombie:\n"
        "Enemy of grass levels."
    );
    zombieText.setPosition({
        enemyTextX,
        zombieY
        });
    window.draw(zombieText);

    sf::Sprite skeletonSprite = skeletonRulesSprite;
    skeletonSprite.setScale({ enemyScale, enemyScale });
    skeletonSprite.setPosition({ enemyIconX, skeletonY + 35.0f });
    window.draw(skeletonSprite);

    sf::Text skeletonText(font);
    skeletonText.setCharacterSize(21);
    skeletonText.setFillColor(sf::Color::White);
    skeletonText.setLineSpacing(1.08f);
    skeletonText.setString(
        "Skeleton:\n"
        "Enemy of desert levels."
    );
    skeletonText.setPosition({
        enemyTextX,
        skeletonY
        });
    window.draw(skeletonText);

    sf::Sprite demonSprite = demonRulesSprite;
    demonSprite.setScale({ enemyScale, enemyScale });
    demonSprite.setPosition({ enemyIconX, demonY + 35.0f });
    window.draw(demonSprite);

    sf::Text demonText(font);
    demonText.setCharacterSize(21);
    demonText.setFillColor(sf::Color::White);
    demonText.setLineSpacing(1.08f);
    demonText.setString(
        "Demon:\n"
        "Enemy of stone levels."
    );
    demonText.setPosition({
        enemyTextX,
        demonY
        });
    window.draw(demonText);

    sf::Text chestText(font);
    chestText.setCharacterSize(20);
    chestText.setFillColor(sf::Color::White);
    chestText.setLineSpacing(1.08f);

    chestText.setString(
        "Chest:\n"
        "Each map contains one chest.\n"
        "It may restore full HP,\n"
        "give +1 attack for 60 seconds,\n"
        "or be empty."
    );

    chestText.setPosition({
        enemyTextX,
        centerY + 165.0f
        });

    window.draw(chestText);

    sf::Text bottomText(font);
    bottomText.setString(
        "Press ESC, ENTER, SPACE or CLICK to return."
    );

    bottomText.setCharacterSize(18);
    bottomText.setFillColor(sf::Color(255, 220, 120));

    sf::FloatRect bottomBounds = bottomText.getLocalBounds();
    bottomText.setOrigin({
        bottomBounds.position.x + bottomBounds.size.x / 2.0f,
        bottomBounds.position.y + bottomBounds.size.y / 2.0f
        });

    bottomText.setPosition({
        centerX,
        centerY + panelHeight / 2.0f - 30.0f
        });

    window.draw(bottomText);

    window.display();
}

bool GameRenderer::isStartButtonClicked(
    sf::Vector2f mousePos
) {
    return startButtonSprite
        .getGlobalBounds()
        .contains(mousePos);
}

bool GameRenderer::isExitButtonClicked(
    sf::Vector2f mousePos
) {
    return exitButtonSprite
        .getGlobalBounds()
        .contains(mousePos);
}

bool GameRenderer::isRulesButtonClicked(
    sf::Vector2f mousePos
) {
    return rulesButtonSprite
        .getGlobalBounds()
        .contains(mousePos);
}

bool GameRenderer::isVictoryButtonClicked(
    sf::Vector2f mousePos
) {
    return victoryButtonSprite
        .getGlobalBounds()
        .contains(mousePos);
}

bool GameRenderer::isGameOverButtonClicked(
    sf::Vector2f mousePos
) {
    return wastedButtonSprite
        .getGlobalBounds()
        .contains(mousePos);
}

sf::Color GameRenderer::getBackgroundColorForLevel(
    int currentLevel
) {
    if (currentLevel <= 5) {
        return RenderColors::GrassBackground;
    }

    if (currentLevel <= 10) {
        return RenderColors::DesertBackground;
    }

    return RenderColors::StoneBackground;
}

void GameRenderer::renderGameAreaBackground(
    sf::RenderWindow& window,
    float gameAreaWidth,
    float gameAreaLeft,
    sf::Color backgroundColor
) {
    sf::RectangleShape gameArea({
        gameAreaWidth,
        static_cast<float>(window.getSize().y)
        });

    gameArea.setPosition({
        gameAreaLeft,
        0.0f
        });

    gameArea.setFillColor(backgroundColor);

    window.draw(gameArea);
}

void GameRenderer::renderObjectOnCell(
    sf::RenderWindow& window,
    const Map& map,
    sf::Sprite& sprite,
    bool visible,
    int cellIndex
) {
    if (!visible || cellIndex == -1) {
        return;
    }

    const HexCell& cell =
        map.getCell(cellIndex);

    sprite.setPosition({
        cell.x,
        cell.y
        });

    window.draw(sprite);
}

void GameRenderer::renderRewardPopup(
    sf::RenderWindow& window,
    const sf::Font& font,
    bool fontLoaded,
    bool popupVisible,
    const std::string& popupTitle,
    const std::string& popupText
) {
    if (!popupVisible || !fontLoaded) {
        return;
    }

    float windowWidth =
        static_cast<float>(window.getSize().x);

    float windowHeight =
        static_cast<float>(window.getSize().y);

    sf::RectangleShape darkOverlay({
        windowWidth,
        windowHeight
        });

    darkOverlay.setPosition({
        0.0f,
        0.0f
        });

    darkOverlay.setFillColor(
        RenderColors::PopupOverlay
    );

    window.draw(darkOverlay);

    float boxWidth = 650.0f;
    float boxHeight = 260.0f;

    float boxX =
        windowWidth / 2.0f - boxWidth / 2.0f;

    float boxY =
        windowHeight / 2.0f - boxHeight / 2.0f;

    sf::RectangleShape box({
        boxWidth,
        boxHeight
        });

    box.setPosition({
        boxX,
        boxY
        });

    box.setFillColor(
        RenderColors::PopupBackground
    );

    box.setOutlineColor(
        RenderColors::PopupOutline
    );

    box.setOutlineThickness(3.0f);

    window.draw(box);

    sf::Text titleText(font);

    titleText.setString(popupTitle);
    titleText.setCharacterSize(36);
    titleText.setFillColor(RenderColors::TextWhite);

    sf::FloatRect titleBounds =
        titleText.getLocalBounds();

    titleText.setOrigin({
        titleBounds.position.x + titleBounds.size.x / 2.0f,
        titleBounds.position.y + titleBounds.size.y / 2.0f
        });

    titleText.setPosition({
        windowWidth / 2.0f,
        boxY + 55.0f
        });

    window.draw(titleText);

    sf::Text messageText(font);

    messageText.setString(popupText);
    messageText.setCharacterSize(24);
    messageText.setFillColor(RenderColors::TextWhite);
    messageText.setLineSpacing(1.15f);

    sf::FloatRect messageBounds =
        messageText.getLocalBounds();

    messageText.setOrigin({
        messageBounds.position.x + messageBounds.size.x / 2.0f,
        messageBounds.position.y + messageBounds.size.y / 2.0f
        });

    messageText.setPosition({
        windowWidth / 2.0f,
        boxY + 130.0f
        });

    window.draw(messageText);

    float buttonWidth = 220.0f;
    float buttonHeight = 55.0f;

    sf::RectangleShape okButton({
        buttonWidth,
        buttonHeight
        });

    okButton.setPosition({
        windowWidth / 2.0f - buttonWidth / 2.0f,
        boxY + boxHeight - 75.0f
        });

    okButton.setFillColor(
        RenderColors::OkButton
    );

    window.draw(okButton);

    sf::Text okText(font);

    okText.setString("OK");
    okText.setCharacterSize(30);
    okText.setFillColor(RenderColors::TextWhite);

    sf::FloatRect okBounds =
        okText.getLocalBounds();

    okText.setOrigin({
        okBounds.position.x + okBounds.size.x / 2.0f,
        okBounds.position.y + okBounds.size.y / 2.0f
        });

    okText.setPosition({
        windowWidth / 2.0f,
        boxY + boxHeight - 47.0f
        });

    window.draw(okText);
}

void GameRenderer::render(
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
) {
    window.clear(sf::Color::Black);

    sf::Color backgroundColor =
        getBackgroundColorForLevel(currentLevel);

    renderGameAreaBackground(
        window,
        gameAreaWidth,
        gameAreaLeft,
        backgroundColor
    );

    map.render(window);

    renderObjectOnCell(
        window,
        map,
        chestSprite,
        chestVisible,
        chestCellIndex
    );

    renderObjectOnCell(
        window,
        map,
        stairsSprite,
        stairsVisible,
        stairsCellIndex
    );

    for (auto& enemy : enemies) {
        enemy->render(window, map);
    }

    hero.render(window, map);

    hero.renderStatusPanel(
        window,
        map,
        font,
        fontLoaded
    );

    renderRewardPopup(
        window,
        font,
        fontLoaded,
        rewardPopupVisible,
        rewardPopupTitle,
        rewardPopupText
    );

    window.display();
}