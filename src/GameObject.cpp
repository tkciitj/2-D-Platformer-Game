#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>

class GameObject
{
private:
    std::shared_ptr<sf::Texture> texture;  // Shared texture to avoid copy issues
    sf::Sprite sprite;

public:
    // Constructor
    GameObject(const std::string& texturePath)
        : texture(std::make_shared<sf::Texture>())
    {
        if (!texture->loadFromFile(texturePath)) {
            throw std::runtime_error("Failed to load texture: " + texturePath);
        }
        sprite.setTexture(*texture);
    }

    // 🚫 Prevent Copying (Since SFML Textures cannot be copied)
    GameObject(const GameObject&) = delete;
    GameObject& operator=(const GameObject&) = delete;

    // ✅ Allow Moving (Efficiently move texture & sprite)
    GameObject(GameObject&& other) noexcept
        : texture(std::move(other.texture)), sprite(std::move(other.sprite)) {}

    GameObject& operator=(GameObject&& other) noexcept
    {
        if (this != &other) {
            texture = std::move(other.texture);
            sprite = std::move(other.sprite);
        }
        return *this;
    }

    // Position & Transformation Methods
    void setPosition(float x, float y) { sprite.setPosition(x, y); }
    void setScale(float scaleX, float scaleY) { sprite.setScale(scaleX, scaleY); }
    void move(float offsetX, float offsetY) { sprite.move(offsetX, offsetY); }

    // Getter Methods
    sf::Vector2f getPosition() const { return sprite.getPosition(); }
    sf::Vector2u getSize() const { return texture->getSize(); }
    sf::Sprite& getSprite() { return sprite; }

    // Set Texture Rect (for animations)
    void setTextureRect(const sf::IntRect& rect) { sprite.setTextureRect(rect); }

    // Draw Function
    void draw(sf::RenderWindow& window) const { window.draw(sprite); }
};

#endif
