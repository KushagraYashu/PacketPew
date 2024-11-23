//author: Kushagra

//including player header file
#include "Player.h"

//custom headers


//cpp headers
#include <algorithm>
#include <iostream>

Player::Player() {
	//default constructor
	m_moveRate = 1.f;
}

Player::Player(sf::Texture& playerTex, sf::Texture& gunTex, float moveRate, sf::RenderWindow& window) {
	SetPlayerSprite(playerTex, window);
	SetGunSprite(gunTex);
	SetMoveRate(moveRate);
}

void Player::SetPlayerSprite(sf::Texture& playerTex, sf::RenderWindow& window) {
	m_playerSprite.setTexture(playerTex); //setting the texture
	m_playerSprite.setOrigin(m_playerSprite.getLocalBounds().width / 2, m_playerSprite.getLocalBounds().height / 2); //setting the origin
	m_playerSprite.setPosition((float)window.getSize().x / 2, (float)window.getSize().y / 2); //setting position to window center
	m_gunOffset = sf::Vector2f(m_playerSprite.getLocalBounds().width / 2, 0);
	m_minX += m_playerSprite.getLocalBounds().width / 2;
	m_minY += m_playerSprite.getLocalBounds().height / 2;
	m_maxX = window.getSize().x - m_minX;
	m_maxY = window.getSize().y - m_minY;
}

void Player::SetGunSprite(sf::Texture& gunTexture) {
	m_gunSprite.setTexture(gunTexture);
	m_gunSprite.setOrigin(m_gunSprite.getLocalBounds().width / 2, m_gunSprite.getLocalBounds().height / 2);
	m_gunSprite.setPosition(m_playerSprite.getPosition());
}

sf::Sprite& Player::GetPlayerSprite()
{
	return m_playerSprite;
}

void Player::SetPlayerPosition(sf::Vector2f pos) {
	m_playerSprite.setPosition(pos);
	m_gunSprite.setPosition(m_playerSprite.getPosition());
}

void Player::UpdatePlayerRotation(sf::RenderWindow& window) {
	sf::Vector2f playerPos = m_playerSprite.getPosition();
	sf::Vector2i mousePos = sf::Mouse::getPosition(window);
	float angle = std::atan2(mousePos.y - playerPos.y, mousePos.x - playerPos.x);
	float angleDegrees = angle * 180.f / 3.14159f;
	m_playerSprite.setRotation(angleDegrees);
	m_gunSprite.setRotation(angleDegrees);
}

void Player::Fire(sf::Texture& bulletTex) {
	CreateBullet(m_bullets, bulletTex, m_playerSprite.getPosition(), m_playerSprite.getRotation(), m_bulletSpeed);
}

void Player::draw(sf::RenderWindow& window, float deltaTime) {
	UpdatePlayerRotation(window);
	window.draw(m_playerSprite);
	UpdateBullets(m_bullets, deltaTime);
	RenderBullets(m_bullets, window);
	window.draw(m_gunSprite);
}

void Player::SetMoveRate(float moveRate) {
	m_moveRate = moveRate;
}

float Player::GetMoveRate() {
	return m_moveRate;
}

void Player::PerformMove(int directionX, int directionY) {
	int dx = (directionX != 0) ? directionX / abs(directionX) : 0;
	int dy = (directionY != 0) ? directionY / abs(directionY) : 0;
	float length = sqrt((dx * dx) + (dy * dy));
	sf::Vector2f direction = sf::Vector2f(dx / length, dy / length);
	sf::Vector2f newPos = m_playerSprite.getPosition() + sf::Vector2f(direction.x * m_moveRate, direction.y * m_moveRate);
	float clampedPosX = std::clamp(newPos.x, m_minX, m_maxX);
	float clampedPosY = std::clamp(newPos.y, m_minY, m_maxY);
	m_playerSprite.setPosition(clampedPosX, clampedPosY);
	m_gunSprite.setPosition(m_playerSprite.getPosition());
}

void Player::MoveLeft(bool left) {
	m_moveLeft = left;
}

void Player::MoveRight(bool right) {
	m_moveRight = right;
}

void Player::MoveUp(bool up) {
	m_moveUp = up;
}

void Player::MoveDown(bool Down) {
	m_moveDown = Down;
}

void Player::CheckMove() {
	if (m_moveUp) {
		PerformMove(0.f, -1.f);
	}
	if (m_moveDown) {
		PerformMove(0.f, 1.f);
	}
	if (m_moveRight) {
		PerformMove(1.f, 0.f);
	}
	if (m_moveLeft) {
		PerformMove(-1.f, 0.f);
	}
}