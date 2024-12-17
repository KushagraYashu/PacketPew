// Author: Kushagra

// Bullet header file

#pragma once

// Standard C++ headers
#include <iostream>

// SFML headers
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

// Struct representing a bullet
struct Bullet {
public:
    sf::Sprite m_bulletSprite;  // Sprite for the bullet
    std::string id;             // Owner identifier for the bullet
    sf::Vector2f m_velocity;    // Velocity vector
    float m_speed;              // Speed of the bullet
    float m_lifetime;           // Remaining lifetime of the bullet
};

// Damage dealt by a bullet
static const float BULLET_DAMAGE = 10.f;

// Creates a new bullet and adds it to the bullets vector
void CreateBullet(std::vector<Bullet>& bullets, sf::Texture& bulletTex, sf::Vector2f spawnPos, std::string id, float spawnRot, float speed, float lifetime = 5.f);

// Updates the positions and lifetimes of bullets
void UpdateBullets(std::vector<Bullet>& bullets, float deltaTime);

// Renders all active bullets to the specified window
void RenderBullets(std::vector<Bullet>& bullets, sf::RenderWindow& window);
