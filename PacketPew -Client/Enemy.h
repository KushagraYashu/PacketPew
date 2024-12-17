// author: Kushagra

// Enemy header file

#pragma once

// Custom headers
#include "Bullet.h"

// Standard C++ headers
#include <iostream>

// SFML headers
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

class Enemy {
public:
    // Constructors
    Enemy(); // Default constructor
    Enemy(sf::Texture& playerTexture, sf::Texture& gunTex, float moveRate, sf::RenderWindow& window); // Parameterized constructor

    // Initialization methods
    void SetAll(sf::Texture& playerTexture, sf::Texture& gunTex, float moveRate, sf::RenderWindow& window); // Initialize all attributes of the enemy
    void SetEnemySprite(sf::Texture& playerTexture, sf::RenderWindow& window); // Configure enemy sprite (texture, origin, position)
    void SetGunSprite(sf::Texture& gunTexture); // Configure gun sprite (texture, origin, position)

    // Accessors and mutators
    sf::Sprite& GetEnemySprite(); // Get a reference to the enemy sprite
    void SetEnemyPosition(sf::Vector2f pos); // Set the enemy's position
    void UpdateEnemyRotation(float angle); // Update the enemy's rotation
    void TakeDamage(float damage); // Apply damage to the enemy
    float GetHealth(); // Get the current health of the enemy
    float GetMoveRate(); // Get the movement rate of the enemy
    void SetMoveRate(float moveRate); // Set the movement rate of the enemy

    // Action methods
    void Fire(sf::Texture& bulletTex, float angle); // Fire a bullet from the enemy's position
    sf::Vector2f MovePredicted(sf::Vector2f moveDir); // Predict the next movement based on direction
    void draw(sf::RenderWindow& window, float deltaTime, sf::Vector2f position, float angle); // Render the enemy and bullets

public:
    std::string m_id = "enemy"; // Identifier for the enemy

private:
    // Enemy attributes
    sf::Sprite m_enemySprite; // The main sprite for the enemy
    sf::Sprite m_gunSprite; // The sprite for the enemy's gun
    sf::Vector2f m_gunOffset; // Offset for the gun sprite position relative to the enemy
    std::vector<Bullet> m_bullets; // Bullets fired by the enemy
    float m_bulletSpeed = 300.f; // Speed of fired bullets
    float m_health = 100.f; // Enemy's health

    // Movement constraints
    float m_moveRate; // Movement speed
    float m_minX = 29.f; // Minimum X position (boundary)
    float m_minY = 29.f; // Minimum Y position (boundary)
    float m_maxX; // Maximum X position (boundary)
    float m_maxY; // Maximum Y position (boundary)
};
