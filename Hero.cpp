#include "Hero.h"
#include "TextureUtils.h"

#include <iostream>
#include <string>

Hero::Hero(
    const std::string& textureFile,
    int startCellIndex
)
    : currentCellIndex(startCellIndex),
    hp(3),
    maxHp(3),
    baseDamage(1),
    damage(1),
    alive(true),
    attackBoostActive(false),
    attackBoostDurationSeconds(60.0f),
    sprite(texture)
{
    texture = loadTextureTransparentBlack(textureFile);

    sprite.setTexture(texture, true);

    sprite.setOrigin({
        texture.getSize().x / 2.0f,
        texture.getSize().y / 2.0f
        });

    sprite.setScale({
        2.50f,
        2.30f
        });

    if (!fullHeartTexture.loadFromFile("heart_full.png")) {
        std::cout << "Cannot load heart_full.png" << std::endl;
    }

    if (!emptyHeartTexture.loadFromFile("heart_empty.png")) {
        std::cout << "Cannot load heart_empty.png" << std::endl;
    }
}

void Hero::tryMoveTo(
    int targetCellIndex,
    const Map& map
) {
    if (!alive) {
        return;
    }

    if (targetCellIndex == -1) {
        return;
    }

    if (map.isCellBlocked(targetCellIndex)) {
        return;
    }

    if (!map.areCellsNeighbours(
        currentCellIndex,
        targetCellIndex
    )) {
        return;
    }

    currentCellIndex = targetCellIndex;
}

void Hero::takeDamage(int amount) {
    if (!alive) {
        return;
    }

    hp -= amount;

    if (hp <= 0) {
        hp = 0;
        alive = false;
    }
}

void Hero::healFull() {
    if (!alive) {
        return;
    }

    hp = maxHp;
}

void Hero::activateAttackBoost() {
    if (!alive) {
        return;
    }

    attackBoostActive = true;
    damage = baseDamage + 1;
    attackBoostClock.restart();
}

void Hero::updateTemporaryEffects() {
    if (!attackBoostActive) {
        return;
    }

    if (attackBoostClock.getElapsedTime().asSeconds() >= attackBoostDurationSeconds) {
        attackBoostActive = false;
        damage = baseDamage;
    }
}

bool Hero::isAlive() const {
    return alive;
}

int Hero::getCurrentCellIndex() const {
    return currentCellIndex;
}

int Hero::getDamage() const {
    return damage;
}

int Hero::getAttackBoostRemainingSeconds() const {
    if (!attackBoostActive) {
        return 0;
    }

    float elapsed =
        attackBoostClock.getElapsedTime().asSeconds();

    float remaining =
        attackBoostDurationSeconds - elapsed;

    if (remaining <= 0.0f) {
        return 0;
    }

    return static_cast<int>(remaining) + 1;
}

void Hero::render(
    sf::RenderWindow& window,
    const Map& map
) {
    if (!alive) {
        return;
    }

    const HexCell& cell =
        map.getCell(currentCellIndex);

    sprite.setPosition({
        cell.x,
        cell.y - 10.0f
        });

    window.draw(sprite);
}

void Hero::renderStatusPanel(
    sf::RenderWindow& window,
    const Map& map,
    const sf::Font& font,
    bool fontLoaded
) {
    if (map.getCellsCount() <= 0) {
        return;
    }

    const float heartScale = 5.0f;
    const float heartSpacing = 1.0f;
    const float gapUnderMap = 20.0f;

    float heartWidth =
        fullHeartTexture.getSize().x * heartScale;

    float heartsTotalWidth =
        maxHp * heartWidth +
        (maxHp - 1) * heartSpacing;

    const HexCell& firstCell =
        map.getCell(0);

    float minMapX = firstCell.x;
    float maxMapX = firstCell.x;
    float maxMapY = firstCell.y;

    for (int i = 0; i < map.getCellsCount(); i++) {
        const HexCell& cell =
            map.getCell(i);

        if (cell.x < minMapX) {
            minMapX = cell.x;
        }

        if (cell.x > maxMapX) {
            maxMapX = cell.x;
        }

        if (cell.y > maxMapY) {
            maxMapY = cell.y;
        }
    }

    float mapLeft =
        minMapX - map.getHexW() / 2.0f;

    float mapRight =
        maxMapX + map.getHexW() / 2.0f;

    float mapWidth =
        mapRight - mapLeft;

    float mapCenterX =
        (mapLeft + mapRight) / 2.0f;

    float panelWidth = 850.0f;

    float panelHeight = 175.0f;

    float panelX =
        mapCenterX - panelWidth / 2.0f;

    float panelY =
        maxMapY + map.getHexH() / 2.0f + gapUnderMap;

    sf::RectangleShape panel({
        panelWidth,
        panelHeight
        });

    panel.setPosition({
        panelX,
        panelY
        });

    panel.setFillColor(sf::Color(93, 0, 0, 255));
    panel.setOutlineColor(sf::Color(93, 0, 0, 255));
    panel.setOutlineThickness(3.0f);

    window.draw(panel);

    float heartsStartX =
        mapCenterX - heartsTotalWidth / 2.0f + 17.0f;

    float heartsStartY =
        panelY + 1.0f;

    for (int i = 0; i < maxHp; i++) {
        sf::Sprite heartSprite(
            i < hp
            ? fullHeartTexture
            : emptyHeartTexture
        );

        heartSprite.setScale({
            heartScale,
            heartScale
            });

        heartSprite.setPosition({
            heartsStartX + i * (heartWidth + heartSpacing),
            heartsStartY
            });

        window.draw(heartSprite);
    }

    if (fontLoaded) {
        sf::Text attackText(font);

        attackText.setString(
            "attack: " + std::to_string(damage)
        );

        attackText.setCharacterSize(30);
        attackText.setFillColor(sf::Color(0, 0, 0));

        sf::FloatRect attackBounds =
            attackText.getLocalBounds();

        attackText.setOrigin({
            attackBounds.position.x + attackBounds.size.x / 2.0f,
            attackBounds.position.y + attackBounds.size.y / 2.0f
            });

        attackText.setPosition({
            panelX + 70.0f,
            panelY + 30.0f
            });

        window.draw(attackText);

        if (attackBoostActive) {
            sf::Text timerText(font);

            timerText.setString(
                "boost: " +
                std::to_string(getAttackBoostRemainingSeconds()) +
                "s"
            );

            timerText.setCharacterSize(30);
            timerText.setFillColor(sf::Color(255, 220, 120));

            sf::FloatRect timerBounds =
                timerText.getLocalBounds();

            timerText.setOrigin({
                timerBounds.position.x + timerBounds.size.x / 2.0f,
                timerBounds.position.y + timerBounds.size.y / 2.0f
                });

            timerText.setPosition({
                panelX + 180.0f,
                panelY + 30.0f
                });

            window.draw(timerText);
        }
    }
}