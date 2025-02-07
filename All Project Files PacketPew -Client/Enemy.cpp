// author: Kushagra

// Including Enemy header file
#include "Enemy.h"

// Standard C++ headers
#include <algorithm>
#include <iostream>
#include <cmath>

// Default constructor
Enemy::Enemy() {
    m_moveRate = 1.f;
    m_health = 100.f; // Default health value
}

// Parameterized constructor
Enemy::Enemy(sf::Texture& playerTex, sf::Texture& gunTex, float moveRate, sf::RenderWindow& window) {
    SetAll(playerTex, gunTex, moveRate, window);
}

// Initialize all enemy attributes
void Enemy::SetAll(sf::Texture& playerTex, sf::Texture& gunTex, float moveRate, sf::RenderWindow& window) {
    SetEnemySprite(playerTex, window);
    SetGunSprite(gunTex);
    SetMoveRate(moveRate);
}

// Set up the enemy sprite with texture and position
void Enemy::SetEnemySprite(sf::Texture& playerTex, sf::RenderWindow& window) {
    m_enemySprite.setTexture(playerTex);
    m_enemySprite.setColor(sf::Color::Magenta); // Magenta color to distinguish the enemy
    m_enemySprite.setOrigin(
        m_enemySprite.getLocalBounds().width / 2,
        m_enemySprite.getLocalBounds().height / 2
    ); // Origin set to center of the sprite
    m_enemySprite.setPosition(static_cast<float>(window.getSize().x) / 2, static_cast<float>(window.getSize().y) / 2);

    m_gunOffset = sf::Vector2f(m_enemySprite.getLocalBounds().width / 2, 0);

    // Calculate boundary constraints for movement
    m_minX = m_enemySprite.getLocalBounds().width / 2;
    m_minY = m_enemySprite.getLocalBounds().height / 2;
    m_maxX = window.getSize().x - m_minX;
    m_maxY = window.getSize().y - m_minY;
}

// Set up the gun sprite with texture and position
void Enemy::SetGunSprite(sf::Texture& gunTexture) {
    m_gunSprite.setTexture(gunTexture);
    m_gunSprite.setOrigin(
        m_gunSprite.getLocalBounds().width / 2,
        m_gunSprite.getLocalBounds().height / 2
    ); // Center origin for correct rotation
    m_gunSprite.setPosition(m_enemySprite.getPosition());
}

// Get a reference to the enemy sprite
sf::Sprite& Enemy::GetEnemySprite() {
    return m_enemySprite;
}

// Update the enemy's position
void Enemy::SetEnemyPosition(sf::Vector2f pos) {
    m_enemySprite.setPosition(pos);
    m_gunSprite.setPosition(pos);
}

// Update the enemy's rotation
void Enemy::UpdateEnemyRotation(float angle) {
    m_enemySprite.setRotation(angle);
    m_gunSprite.setRotation(angle);
}

// Fire a bullet from the enemy's position
void Enemy::Fire(sf::Texture& bulletTex, float angle) {
    UpdateEnemyRotation(angle);
    CreateBullet(m_bullets, bulletTex, m_enemySprite.getPosition(), m_id, m_enemySprite.getRotation(), m_bulletSpeed);
}

// Predict the next position of the enemy based on movement direction
sf::Vector2f Enemy::MovePredicted(sf::Vector2f moveDir) {
    int dx = (moveDir.x != 0) ? moveDir.x / std::abs(moveDir.x) : 0;
    int dy = (moveDir.y != 0) ? moveDir.y / std::abs(moveDir.y) : 0;
    float length = std::sqrt((dx * dx) + (dy * dy));

    sf::Vector2f direction = (length != 0) ? sf::Vector2f(dx / length, dy / length) : sf::Vector2f(0.f, 0.f);
    return sf::Vector2f(direction.x * m_moveRate, direction.y * m_moveRate);
}

// Draw the enemy and its bullets
void Enemy::draw(sf::RenderWindow& window, float deltaTime, sf::Vector2f position, float angle) {
    SetEnemyPosition(position);
    UpdateEnemyRotation(angle);

    UpdateBullets(m_bullets, deltaTime);
    RenderBullets(m_bullets, window);

    window.draw(m_enemySprite);
    window.draw(m_gunSprite);
}

// Apply damage to the enemy
void Enemy::TakeDamage(float damage) {
    m_health -= damage;
}

// Get the enemy's current health
float Enemy::GetHealth() {
    return m_health;
}

// Set the enemy's movement rate
void Enemy::SetMoveRate(float moveRate) {
    m_moveRate = moveRate;
}

// Get the enemy's movement rate
float Enemy::GetMoveRate() {
    return m_moveRate;
}