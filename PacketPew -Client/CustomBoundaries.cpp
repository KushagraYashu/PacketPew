//author: Kushagra

#pragma once

//cpp headers
#include <iostream>

//sfml headers
#include<SFML/Network.hpp>
#include<SFML/Graphics.hpp>

using namespace std;

static void CreateBoundaries(sf::Sprite (&bounds) [4], sf::Texture& boundTex, sf::RenderWindow& window) {
	for (int i = 0;i < 4;i++) {
		//[0], [1] width
		//[2], [3] height
		bounds[i].setTexture(boundTex);
		if (i == 0 || i == 1) {
			bounds[i].setTextureRect(sf::IntRect(0, 0, window.getSize().x, 29));
			if (i == 0) {
				bounds[i].setPosition(sf::Vector2f(0.f, -5.f));
			}
			else {
				bounds[i].setPosition(sf::Vector2f(0.f, window.getSize().y - (29 - 5)));
			}
		}
		if (i == 2 || i == 3) {
			bounds[i].setTextureRect(sf::IntRect(0, 0, window.getSize().y, 29));
			if (i == 2) {
				bounds[i].setPosition(sf::Vector2f(29 - 5, 0.f));
			}
			else {
				bounds[i].setPosition(window.getSize().x + 5, 0.f);
			}
			bounds[i].rotate(90);
		}
	}
}

static void DisplayBoundaries(sf::Sprite(&bounds)[4], sf::RenderWindow& window) {
	for (int i = 0;i < 4;i++) { //boundaries
		window.draw(bounds[i]);
	}
}