//author: Kushagra

//Bullet header file

#pragma once

//cpp headers
#include<iostream>

//sfml headers
#include<SFML/Network.hpp>
#include<SFML/Graphics.hpp>

struct Bullet {
public: 
	sf::Sprite m_bulletSprite;
	sf::Vector2f m_velocity;
	float m_speed;
	float m_lifetime;
};

void CreateBullet(std::vector<Bullet>& bullets, sf::Texture& bulletTex, sf::Vector2f spawnPos, float spawnRot, float speed, float lifetime = 5.f);

void UpdateBullets(std::vector<Bullet>& bullets, float deltaTime);

void RenderBullets(std::vector<Bullet>& bullets, sf::RenderWindow& window);