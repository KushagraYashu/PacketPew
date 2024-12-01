//author: Kushagra

//Player header file

#pragma once

//custom headers
#include "Bullet.h"

//cpp headers
#include<iostream>

//sfml headers
#include<SFML/Network.hpp>
#include<SFML/Graphics.hpp>

class Player {
public: 
	Player(); //default constructor
	Player(sf::Texture& playerTexture, sf::Texture& gunTex, float moveRate, sf::RenderWindow& window); //set the whole player
	
	void SetPlayerSprite(sf::Texture& playerTexture, sf::RenderWindow& window); //set texture, set origin, position to window centre
	void SetGunSprite(sf::Texture& gunTexture); //set texture, origin, and position relative to the player
	sf::Sprite& GetPlayerSprite(); //return a pointer to player sprite
	void SetPlayerPosition(sf::Vector2f pos);
	void UpdatePlayerRotation(sf::RenderWindow& window);
	void Fire(sf::Texture& bulletTex);

	void draw(sf::RenderWindow& window, float deltaTime);

	float GetMoveRate(); //return a copy to move rate
	void SetMoveRate(float moveRate); //set the move rate

	float GetMinX();
	float GetMaxX();
	float GetMinY();
	float GetMaxY();

	void Move(sf::Vector2f newPos);
	void MovePredicted(sf::Vector2f moveDir);
	sf::Vector2f CheckMove();
	void MoveLeft(bool left);
	void MoveRight(bool right);
	void MoveUp(bool up);
	void MoveDown(bool down);

private:

	//player variables
	sf::Sprite m_playerSprite;
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
	bool m_moveLeft = false;
	bool m_moveRight = false;
	bool m_moveUp = false;
	bool m_moveDown = false;

};