#include "TextureUtils.h"

#include <stdexcept>

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