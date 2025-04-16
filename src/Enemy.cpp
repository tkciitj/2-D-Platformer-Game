#include "Enemy.h"
#include <cmath>

Enemy::Enemy(sf::Texture& textureR, sf::Texture& textureL, sf::Vector2f spawnPos, float speed)
    : textureRight(&textureR), textureLeft(&textureL), speed(speed), isActive(true), lastX(spawnPos.x) {
    sprite.setTexture(textureR); // Default to right-facing
    sprite.setPosition(spawnPos);
    attackClock.restart(); // Start cooldown timer
}

void Enemy::update(sf::Vector2f playerPos) {
    if (!isActive) return;

    sf::Vector2f dir = playerPos - sprite.getPosition();
    float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (length != 0)
        dir /= length;

    sprite.move(dir * speed);

    float currentX = sprite.getPosition().x;

    // Flip sprite texture based on movement direction
    if (std::abs(currentX - lastX) > 0.1f) {
        if (currentX > lastX) {
            sprite.setTexture(*textureRight);
        } else {
            sprite.setTexture(*textureLeft);
        }
    }

    lastX = currentX;
}

sf::FloatRect Enemy::getBounds() const {
    return sprite.getGlobalBounds();
}

sf::Sprite& Enemy::getSprite() {
    return sprite;
}

void Enemy::deactivate() {
    isActive = false;
}

bool Enemy::isAlive() const {
    return isActive;
}

// Cooldown check
bool Enemy::canAttack() {
    return attackClock.getElapsedTime() >= attackCooldown;
}

// Reset cooldown after attack
void Enemy::resetAttackCooldown() {
    attackClock.restart();
}

void Enemy::takeHit() {
    hitCount++;
    if (hitCount >= 3) {
        deactivate();  // Set isActive = false
    }
}
