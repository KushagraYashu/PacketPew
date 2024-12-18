//author: Kushagra

//Player header file

#pragma once

// Custom headers
#include "Bullet.h"

// Standard C++ headers
#include <iostream>

// SFML headers
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

class Player {
public:
    Player();
    // Default constructor

    Player(sf::Texture& playerTexture, sf::Texture& gunTex, float moveRate, sf::RenderWindow& window);
    // Parameterized constructor to initialize player and gun sprites, and movement rate

    void SetPlayerSprite(sf::Texture& playerTexture, sf::RenderWindow& window);
    // Sets up the player sprite: texture, origin, and initial position at the center of the window

    void SetGunSprite(sf::Texture& gunTexture);
    // Sets up the gun sprite: texture, origin, and position relative to the player

    sf::Sprite& GetPlayerSprite();
    // Returns a reference to the player sprite

    sf::Sprite& GetGunSprite() { return m_gunSprite; }
    // Returns a reference to the gun sprite

    void SetPlayerPosition(sf::Vector2f pos);
    // Sets the player's position and aligns the gun position accordingly

    void UpdatePlayerRotation(sf::RenderWindow& window);
    // Updates the rotation of the player and gun based on mouse position

    void Fire(sf::Texture& bulletTex);
    // Fires a bullet from the player's position and rotation

    void draw(sf::RenderWindow& window, float deltaTime);
    // Draws the player, gun, and bullets to the window

    float GetMoveRate();
    // Returns the player's movement rate

    void SetMoveRate(float moveRate);
    // Sets the player's movement rate

    float GetMinX();
    // Returns the minimum X boundary

    float GetMaxX();
    // Returns the maximum X boundary

    float GetMinY();
    // Returns the minimum Y boundary

    float GetMaxY();
    // Returns the maximum Y boundary

    float GetHealth();
    // Returns the player's current health

    void TakeDamage(float damage);
    // Reduces the player's health by the given damage value

    void Move(sf::Vector2f newPos);
    // Moves the player to a new position, clamping it within defined boundaries

    void MovePredicted(sf::Vector2f moveDir);
    // Predicts the player's movement in a specific direction and updates the position

    sf::Vector2f CheckMove();
    // Returns the player's movement direction based on input states

    void MoveLeft(bool left);
    // Sets the left movement flag

    void MoveRight(bool right);
    // Sets the right movement flag

    void MoveUp(bool up);
    // Sets the up movement flag

    void MoveDown(bool down);
    // Sets the down movement flag

public:
    std::string m_id = "player";
    // Player ID string for identification

    float m_bulletSpeed = 300.f;
    // Bullet speed

private:
    // Player variables
    sf::Sprite m_playerSprite;
    // Sprite representing the player

    sf::Sprite m_gunSprite;
    // Sprite representing the player's gun

    sf::Vector2f m_gunOffset;
    // Offset for gun position relative to the player

    std::vector<Bullet> m_bullets;
    // Vector storing bullets fired by the player

    float m_health = 100.f;
    // Player's health

    float m_moveRate;
    // Player's movement rate

    // Movement boundaries
    float m_minX = 29.f;
    float m_minY = 29.f;
    float m_maxX;
    float m_maxY;

    // Movement flags
    bool m_moveLeft = false;
    bool m_moveRight = false;
    bool m_moveUp = false;
    bool m_moveDown = false;
};