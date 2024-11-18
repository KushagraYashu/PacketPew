//author: Kushagra

//including bullet header file
#include "Bullet.h"

void CreateBullet(std::vector<Bullet>& bullets, sf::Texture& bulletTex, sf::Vector2f spawnPos, float spawnRot, float speed, float lifetime) {
	Bullet bullet;
	bullet.m_speed = speed;
	bullet.m_bulletSprite.setTexture(bulletTex);
	bullet.m_bulletSprite.setOrigin(bullet.m_bulletSprite.getLocalBounds().width / 2, bullet.m_bulletSprite.getLocalBounds().height / 2);
	bullet.m_bulletSprite.setPosition(spawnPos);
	bullet.m_bulletSprite.setRotation(spawnRot);
	float angleRad = spawnRot * 3.14159f / 180.f;
	bullet.m_velocity = sf::Vector2f(std::cos(angleRad) * bullet.m_speed, std::sin(angleRad) * bullet.m_speed);
	bullet.m_lifetime = lifetime;
	bullets.push_back(bullet);
}

void UpdateBullets(std::vector<Bullet>& bullets, float deltaTime) {
	for (auto it = bullets.begin(); it != bullets.end();) {
		it->m_bulletSprite.move(it->m_velocity * deltaTime);
		it->m_lifetime -= deltaTime;
		if (it->m_lifetime <= 0) {
			it = bullets.erase(it);
		}
		else {
			++it;
		}
	}
}

void RenderBullets(std::vector<Bullet>& bullets, sf::RenderWindow& window) {
	for (const auto& bullet : bullets) {
		window.draw(bullet.m_bulletSprite);
	}
}
