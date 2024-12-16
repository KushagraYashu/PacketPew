//author: Kushagra

//including Enemy header file
#include "Enemy.h"

//custom headers


//cpp headers
#include <algorithm>
#include <iostream>

Enemy::Enemy() {
	//default constructor
	m_moveRate = 1.f;
}

Enemy::Enemy(sf::Texture& playerTex, sf::Texture& gunTex, float moveRate, sf::RenderWindow& window) {
	SetEnemySprite(playerTex, window);
	SetGunSprite(gunTex);
	SetMoveRate(moveRate);
}

void Enemy::SetAll(sf::Texture& playerTex, sf::Texture& gunTex, float moveRate, sf::RenderWindow& window) {
	SetEnemySprite(playerTex, window);
	SetGunSprite(gunTex);
	SetMoveRate(moveRate);
}

void Enemy::SetEnemySprite(sf::Texture& playerTex, sf::RenderWindow& window) {
	m_enemySprite.setTexture(playerTex); //setting the texture
	m_enemySprite.setColor(sf::Color::Magenta);
	m_enemySprite.setOrigin(m_enemySprite.getLocalBounds().width / 2, m_enemySprite.getLocalBounds().height / 2); //setting the origin
	m_enemySprite.setPosition((float)window.getSize().x / 2, (float)window.getSize().y / 2); //setting position to window center
	m_gunOffset = sf::Vector2f(m_enemySprite.getLocalBounds().width / 2, 0);
	m_minX += m_enemySprite.getLocalBounds().width / 2;
	m_minY += m_enemySprite.getLocalBounds().height / 2;
	m_maxX = window.getSize().x - m_minX;
	m_maxY = window.getSize().y - m_minY;
}

void Enemy::SetGunSprite(sf::Texture& gunTexture) {
	m_gunSprite.setTexture(gunTexture);
	m_gunSprite.setOrigin(m_gunSprite.getLocalBounds().width / 2, m_gunSprite.getLocalBounds().height / 2);
	m_gunSprite.setPosition(m_enemySprite.getPosition());
}

sf::Sprite& Enemy::GetEnemySprite()
{
	return m_enemySprite;
}

void Enemy::SetEnemyPosition(sf::Vector2f pos) {
	m_enemySprite.setPosition(pos);
	m_gunSprite.setPosition(m_enemySprite.getPosition());
}

void Enemy::UpdateEnemyRotation(float angle) {
	m_enemySprite.setRotation(angle);
	m_gunSprite.setRotation(angle);
}

void Enemy::Fire(sf::Texture& bulletTex, float angle) {
	UpdateEnemyRotation(angle);
	CreateBullet(m_bullets, bulletTex, m_enemySprite.getPosition(), m_enemySprite.getRotation(), m_bulletSpeed);
}

sf::Vector2f Enemy::MovePredicted(sf::Vector2f  moveDir) {
	int dx = (moveDir.x != 0) ? moveDir.x / abs(moveDir.x) : 0;
	int dy = (moveDir.y != 0) ? moveDir.y / abs(moveDir.y) : 0;
	float length = sqrt((dx * dx) + (dy * dy));
	sf::Vector2f direction = sf::Vector2f((length !=0) ? dx / length : 0, (length != 0) ? dy / length : 0);
	sf::Vector2f newPos = sf::Vector2f(direction.x * m_moveRate, direction.y * m_moveRate);
	return newPos;
}

void Enemy::draw(sf::RenderWindow& window, float deltaTime, sf::Vector2f position, float angle) {
	SetEnemyPosition(position);
	UpdateEnemyRotation(angle);
	UpdateBullets(m_bullets, deltaTime);
	RenderBullets(m_bullets, window);
	window.draw(m_enemySprite);
	window.draw(m_gunSprite);
}

void Enemy::SetMoveRate(float moveRate) {
	m_moveRate = moveRate;
}

float Enemy::GetMoveRate() {
	return m_moveRate;
}