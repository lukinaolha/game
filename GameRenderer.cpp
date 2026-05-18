#include "GameRenderer.h"

namespace RenderColors {
    const sf::Color GrassBackground(180, 180, 180, 120);
    const sf::Color DesertBackground(196, 122, 58, 255);
    const sf::Color StoneBackground(50, 41, 71, 255);

    const sf::Color PopupOverlay(0, 0, 0, 180);
    const sf::Color PopupBackground(25, 25, 25, 245);
    const sf::Color PopupOutline(220, 220, 220, 255);

    const sf::Color TextWhite(255, 255, 255, 255);

    const sf::Color OkButton(93, 0, 0, 255);
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

    const HexCell& cell = map.getCell(cellIndex);

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