#include "GameRenderer.h"

#include <iostream>

sf::Texture GameRenderer::menuBackgroundTexture;
sf::Sprite GameRenderer::menuBackgroundSprite(menuBackgroundTexture);

sf::Texture GameRenderer::startButtonTexture;
sf::Sprite GameRenderer::startButtonSprite(startButtonTexture);

sf::Texture GameRenderer::exitButtonTexture;
sf::Sprite GameRenderer::exitButtonSprite(exitButtonTexture);

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

    if (!exitButtonTexture.loadFromFile("exit.png")) {
        std::cout << "Cannot load exit.png" << std::endl;
    }

    exitButtonSprite.setTexture(exitButtonTexture, true);

    float buttonScale = 5.0f;

    startButtonSprite.setScale({
        buttonScale,
        buttonScale
        });

    exitButtonSprite.setScale({
        buttonScale,
        buttonScale
        });

    sf::FloatRect startBounds =
        startButtonSprite.getGlobalBounds();

    sf::FloatRect exitBounds =
        exitButtonSprite.getGlobalBounds();

    float centerX =
        static_cast<float>(windowWidth) / 2.0f;

    startButtonSprite.setPosition({
        centerX - startBounds.size.x / 2.0f,
        static_cast<float>(windowHeight) * 0.58f
        });

    exitButtonSprite.setPosition({
        centerX - exitBounds.size.x / 2.0f,
        static_cast<float>(windowHeight) * 0.70f
        });
}

void GameRenderer::renderMenu(
    sf::RenderWindow& window
) {
    window.clear(sf::Color::Black);

    window.draw(menuBackgroundSprite);
    window.draw(startButtonSprite);
    window.draw(exitButtonSprite);

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