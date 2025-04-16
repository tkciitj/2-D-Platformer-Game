#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>

class Enemy {
public:
    Enemy(sf::Texture& textureR, sf::Texture& textureL, sf::Vector2f spawnPos, float speed = 1.5f);

    void update(sf::Vector2f playerPos);
    sf::FloatRect getBounds() const;
    sf::Sprite& getSprite();

    void takeHit();  // Declare this method
    void deactivate();
    bool isAlive() const;

    bool canAttack();             // Check if cooldown passed
    void resetAttackCooldown();  // Reset cooldown after attack

    bool isActive;

private:
    sf::Sprite sprite;
    sf::Texture* textureRight;
    sf::Texture* textureLeft;
    float speed;
    float lastX; // For direction tracking
    int hitCount = 0;

    // Cooldown logic
    sf::Clock attackClock;
    sf::Time attackCooldown = sf::seconds(1.0f); // 1 second cooldown
};

#endif // ENEMY_H
