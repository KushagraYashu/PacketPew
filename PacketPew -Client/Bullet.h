//author: Kushagra

//Bullet header file

#pragma once

//cpp headers
#include<iostream>

//sfml headers
#include<SFML/Network.hpp>
#include<SFML/Graphics.hpp>

class Player; //forward declarations
class Enemy;

struct Bullet {
public: 
	sf::Sprite m_bulletSprite;
	std::string id;
	sf::Vector2f m_velocity;
	float m_speed;
	float m_lifetime;
};

static const float BULLET_DAMAGE = 10.f;

void CreateBullet(std::vector<Bullet>& bullets, sf::Texture& bulletTex, sf::Vector2f spawnPos, std::string id, float spawnRot, float speed, float lifetime = 5.f);

void UpdateBullets(std::vector<Bullet>& bullets, float deltaTime, bool check, Player* player = nullptr, Enemy* enemy = nullptr);

void CheckCollisions(std::vector<Bullet>& bullets, Player* player, Enemy* enemy);

void RenderBullets(std::vector<Bullet>& bullets, sf::RenderWindow& window);