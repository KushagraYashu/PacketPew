// Author: Kushagra

// Include the bullet header file
#include "Bullet.h"

// Creates a new bullet and adds it to the bullets vector.
void CreateBullet(std::vector<Bullet>& bullets, sf::Texture& bulletTex, sf::Vector2f spawnPos, std::string id, float spawnRot, float speed, float lifetime) {
    Bullet bullet;

    // Set bullet properties
    bullet.m_speed = speed;
    bullet.m_bulletSprite.setTexture(bulletTex);
    bullet.m_bulletSprite.setOrigin(bullet.m_bulletSprite.getLocalBounds().width / 2, bullet.m_bulletSprite.getLocalBounds().height / 2);
    bullet.id = id;

    // Set position and rotation
    bullet.m_bulletSprite.setPosition(spawnPos);
    bullet.m_bulletSprite.setRotation(spawnRot);

    // Calculate velocity based on rotation angle
    float angleRad = spawnRot * 3.14159f / 180.f; // Convert degrees to radians
    bullet.m_velocity = sf::Vector2f(std::cos(angleRad) * bullet.m_speed, std::sin(angleRad) * bullet.m_speed);

    // Set bullet lifetime
    bullet.m_lifetime = lifetime;

    // Add bullet to the bullets vector
    bullets.push_back(bullet);
}

// Updates the positions and lifetimes of all active bullets.
void UpdateBullets(std::vector<Bullet>& bullets, float deltaTime) {
    // Iterate through bullets and update their properties
    for (auto it = bullets.begin(); it != bullets.end();) {
        // Move bullet based on its velocity
        it->m_bulletSprite.move(it->m_velocity * deltaTime);

        // Decrease lifetime
        it->m_lifetime -= deltaTime;

        // Remove bullets whose lifetime has expired
        if (it->m_lifetime <= 0) {
            it = bullets.erase(it);
        }
        else {
            ++it; // Move to the next bullet
        }
    }
}

// Renders all active bullets to the game window.
void RenderBullets(std::vector<Bullet>& bullets, sf::RenderWindow& window) {
	for (const auto& bullet : bullets) {
		window.draw(bullet.m_bulletSprite);
	}
}