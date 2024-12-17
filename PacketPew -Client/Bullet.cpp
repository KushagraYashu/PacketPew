//author: Kushagra

//custom header files
#include "Player.h"
#include "Enemy.h"

//including bullet header file
#include "Bullet.h"

void CreateBullet(std::vector<Bullet>& bullets, sf::Texture& bulletTex, sf::Vector2f spawnPos, std::string id, float spawnRot, float speed, float lifetime) {
	Bullet bullet;
	bullet.m_speed = speed;
	bullet.m_bulletSprite.setTexture(bulletTex);
	bullet.m_bulletSprite.setOrigin(bullet.m_bulletSprite.getLocalBounds().width / 2, bullet.m_bulletSprite.getLocalBounds().height / 2);
	bullet.id = id;
	bullet.m_bulletSprite.setPosition(spawnPos);
	bullet.m_bulletSprite.setRotation(spawnRot);
	float angleRad = spawnRot * 3.14159f / 180.f;
	bullet.m_velocity = sf::Vector2f(std::cos(angleRad) * bullet.m_speed, std::sin(angleRad) * bullet.m_speed);
	bullet.m_lifetime = lifetime;
	bullets.push_back(bullet);
}

void UpdateBullets(std::vector<Bullet>& bullets, float deltaTime, bool check, Player* player, Enemy* enemy) {
	/*if (check) {
		CheckCollisions(bullets, player, enemy);
	}*/
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

void CheckCollisions(std::vector<Bullet>& bullets, Player* player, Enemy* enemy) {
	for (auto it = bullets.begin(); it != bullets.end();) {
		sf::FloatRect bulletBounds = it->m_bulletSprite.getGlobalBounds();
		
		if (bulletBounds.intersects(player->GetPlayerSprite().getGlobalBounds())) {
			//it = bullets.erase(it); // Remove the bullet
			//continue; // Skip the rest of the checks for this bullet
		}

		try
		{
			if (bulletBounds.intersects(enemy->GetEnemySprite().getGlobalBounds())) {
				std::cout << "hit an enemy\n";
				it = bullets.erase(it);
				continue;
			}
		}
		catch (const std::exception&)
		{

		}

		++it;
	}
}

void RenderBullets(std::vector<Bullet>& bullets, sf::RenderWindow& window) {
	for (const auto& bullet : bullets) {
		window.draw(bullet.m_bulletSprite);
	}
}
