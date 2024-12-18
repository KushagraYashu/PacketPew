//author: Kushagra

//including player header file
#include "Player.h"

//custom headers

//cpp headers
#include <algorithm>
#include <iostream>

// Default constructor
Player::Player() {
    m_moveRate = 1.f; // Initialize default movement rate
}

// Parameterized constructor to set player properties
Player::Player(sf::Texture& playerTex, sf::Texture& gunTex, float moveRate, sf::RenderWindow& window) {
    SetPlayerSprite(playerTex, window); // Set up player sprite
    SetGunSprite(gunTex);              // Set up gun sprite
    SetMoveRate(moveRate);             // Set movement rate
}

// Sets up the player sprite properties
void Player::SetPlayerSprite(sf::Texture& playerTex, sf::RenderWindow& window) {
    m_playerSprite.setTexture(playerTex); // Set texture for the player sprite
    m_playerSprite.setOrigin(m_playerSprite.getLocalBounds().width / 2, m_playerSprite.getLocalBounds().height / 2); // Center origin
    m_playerSprite.setPosition((float)window.getSize().x / 2, (float)window.getSize().y / 2); // Place at the center of the window
    m_gunOffset = sf::Vector2f(m_playerSprite.getLocalBounds().width / 2, 0); // Calculate gun offset
    m_minX += m_playerSprite.getLocalBounds().width / 2; // Adjust boundary constraints
    m_minY += m_playerSprite.getLocalBounds().height / 2;
    m_maxX = window.getSize().x - m_minX;
    m_maxY = window.getSize().y - m_minY;
}

// Sets up the gun sprite properties
void Player::SetGunSprite(sf::Texture& gunTexture) {
    m_gunSprite.setTexture(gunTexture); // Set gun texture
    m_gunSprite.setOrigin(m_gunSprite.getLocalBounds().width / 2, m_gunSprite.getLocalBounds().height / 2); // Center origin
    m_gunSprite.setPosition(m_playerSprite.getPosition()); // Place gun relative to player
}

// Returns a reference to the player sprite
sf::Sprite& Player::GetPlayerSprite() {
    return m_playerSprite;
}

// Sets the player's position and updates the gun position
void Player::SetPlayerPosition(sf::Vector2f pos) {
    m_playerSprite.setPosition(pos);
    m_gunSprite.setPosition(m_playerSprite.getPosition());
}

// Updates the player and gun rotation based on mouse position
void Player::UpdatePlayerRotation(sf::RenderWindow& window) {
    sf::Vector2f playerPos = m_playerSprite.getPosition(); // Get player position
    sf::Vector2i mousePos = sf::Mouse::getPosition(window); // Get mouse position
    float angle = std::atan2(mousePos.y - playerPos.y, mousePos.x - playerPos.x); // Calculate angle in radians
    float angleDegrees = angle * 180.f / 3.14159f; // Convert to degrees
    m_playerSprite.setRotation(angleDegrees); // Set player rotation
    m_gunSprite.setRotation(angleDegrees);    // Set gun rotation
}

// Fires a bullet from the player's position
void Player::Fire(sf::Texture& bulletTex) {
    CreateBullet(m_bullets, bulletTex, m_playerSprite.getPosition(), m_id, m_playerSprite.getRotation(), m_bulletSpeed);
}

// Draws the player, gun, and bullets to the window
void Player::draw(sf::RenderWindow& window, float deltaTime) {
    UpdatePlayerRotation(window);                  // Update rotation before drawing
    window.draw(m_playerSprite);                   // Draw player sprite
    UpdateBullets(m_bullets, deltaTime);           // Update bullet positions
    RenderBullets(m_bullets, window);              // Render bullets
    window.draw(m_gunSprite);                      // Draw gun sprite
}

// Sets the player's movement rate
void Player::SetMoveRate(float moveRate) {
    m_moveRate = moveRate;
}

// Returns the player's movement rate
float Player::GetMoveRate() {
    return m_moveRate;
}

// Returns the minimum X boundary
float Player::GetMinX() {
    return m_minX;
}

// Returns the minimum Y boundary
float Player::GetMinY() {
    return m_minY;
}

// Returns the maximum X boundary
float Player::GetMaxX() {
    return m_maxX;
}

// Returns the maximum Y boundary
float Player::GetMaxY() {
    return m_maxY;
}

// Returns the player's health
float Player::GetHealth() {
    return m_health;
}

// Reduces the player's health by the specified damage amount
void Player::TakeDamage(float damage) {
    m_health -= damage;
}

// Moves the player to the specified position with boundary clamping
void Player::Move(sf::Vector2f pos) {
    sf::Vector2f newPos = m_playerSprite.getPosition() + pos; // Calculate new position
    float clampedPosX = std::clamp(newPos.x, m_minX, m_maxX); // Clamp X position to boundaries
    float clampedPosY = std::clamp(newPos.y, m_minY, m_maxY); // Clamp Y position to boundaries
    m_playerSprite.setPosition(clampedPosX, clampedPosY);     // Update player position
    m_gunSprite.setPosition(m_playerSprite.getPosition());    // Update gun position
}

// Predicts the player's movement based on the direction
void Player::MovePredicted(sf::Vector2f moveDir) {
    int dx = (moveDir.x != 0) ? moveDir.x / abs(moveDir.x) : 0;
    int dy = (moveDir.y != 0) ? moveDir.y / abs(moveDir.y) : 0;
    float length = sqrt((dx * dx) + (dy * dy));
    sf::Vector2f direction = sf::Vector2f(dx / length, dy / length); // Normalize direction
    sf::Vector2f newPos = m_playerSprite.getPosition() + sf::Vector2f(direction.x * m_moveRate, direction.y * m_moveRate);
    float clampedPosX = std::clamp(newPos.x, m_minX, m_maxX);
    float clampedPosY = std::clamp(newPos.y, m_minY, m_maxY);
    m_playerSprite.setPosition(clampedPosX, clampedPosY);
    m_gunSprite.setPosition(m_playerSprite.getPosition());
}

// Setters for movement directions
void Player::MoveLeft(bool left) { m_moveLeft = left; }
void Player::MoveRight(bool right) { m_moveRight = right; }
void Player::MoveUp(bool up) { m_moveUp = up; }
void Player::MoveDown(bool down) { m_moveDown = down; }

// Checks which directions the player is moving and returns the resulting vector
sf::Vector2f Player::CheckMove() {
    sf::Vector2f playerMovDir = sf::Vector2f();
    if (m_moveUp) { playerMovDir += sf::Vector2f(0.f, -1.f); }
    if (m_moveDown) { playerMovDir += sf::Vector2f(0.f, 1.f); }
    if (m_moveRight) { playerMovDir += sf::Vector2f(1.f, 0.f); }
    if (m_moveLeft) { playerMovDir += sf::Vector2f(-1.f, 0.f); }
    return playerMovDir;
}
