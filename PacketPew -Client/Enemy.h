//author: Kushagra

//Enemy header file

#pragma once

//custom headers
#include "Bullet.h"

//cpp headers
#include<iostream>

//sfml headers
#include<SFML/Network.hpp>
#include<SFML/Graphics.hpp>

class Enemy {
public: 
	Enemy(); //default constructor
	Enemy(sf::Texture& playerTexture, sf::Texture& gunTex, float moveRate, sf::RenderWindow& window); //set the whole Enemy

	void SetAll(sf::Texture& playerTexture, sf::Texture& gunTex, float moveRate, sf::RenderWindow& window);
	
	void SetEnemySprite(sf::Texture& playerTexture, sf::RenderWindow& window); //set texture, set origin, position to window centre
	void SetGunSprite(sf::Texture& gunTexture); //set texture, origin, and position relative to the Enemy
	sf::Sprite& GetEnemySprite(); //return a pointer to Enemy sprite
	void SetEnemyPosition(sf::Vector2f pos);
	void UpdateEnemyRotation(float angle);
	void Fire(sf::Texture& bulletTex);

	void draw(sf::RenderWindow& window, float deltaTime, sf::Vector2f position, float angle);

	float GetMoveRate(); //return a copy to move rate
	void SetMoveRate(float moveRate); //set the move rate

private:

	//Enemy variables
	sf::Sprite m_enemySprite;
	sf::Sprite m_gunSprite;
	sf::Vector2f m_gunOffset;
	std::vector<Bullet> m_bullets;
	float m_bulletSpeed = 300.f;

	//move variables
	float m_moveRate;
	float m_minX = 29.f; 
	float m_minY = 29.f; 
	float m_maxX; 
	float m_maxY;

};