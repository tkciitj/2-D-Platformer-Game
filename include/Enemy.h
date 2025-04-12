#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>

class Enemy {
public:
    Enemy(sf::Texture& textureR, sf::Texture& textureL, sf::Vector2f spawnPos, float speed = 1.5f);

    void update(sf::Vector2f playerPos);
    sf::FloatRect getBounds() const;
    sf::Sprite& getSprite();

    void deactivate();
    bool isAlive() const;
    bool isActive;

private:
    sf::Sprite sprite;
    sf::Texture* textureRight;
    sf::Texture* textureLeft;
    float speed;
    float lastX; // for direction tracking
};

#endif // ENEMY_H
