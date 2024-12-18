// Author: Kushagra

#pragma once

// Standard C++ headers
#include <iostream>

// SFML headers
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

using namespace std;

// Creates boundary sprites for the game window.
static void CreateBoundaries(sf::Sprite(&bounds)[4], sf::Texture& boundTex, sf::RenderWindow& window) {
    for (int i = 0; i < 4; i++) {
        // [0], [1] = horizontal boundaries (top and bottom)
        // [2], [3] = vertical boundaries (left and right)
        bounds[i].setTexture(boundTex);

        if (i == 0 || i == 1) { // Horizontal boundaries
            bounds[i].setTextureRect(sf::IntRect(0, 0, window.getSize().x, 29)); // Set width
            if (i == 0) { // Top boundary
                bounds[i].setPosition(sf::Vector2f(0.f, -5.f));
            }
            else { // Bottom boundary
                bounds[i].setPosition(sf::Vector2f(0.f, window.getSize().y - (29 - 5)));
            }
        }

        if (i == 2 || i == 3) { // Vertical boundaries
            bounds[i].setTextureRect(sf::IntRect(0, 0, window.getSize().y, 29)); // Set height
            if (i == 2) { // Left boundary
                bounds[i].setPosition(sf::Vector2f(29 - 5, 0.f));
            }
            else { // Right boundary
                bounds[i].setPosition(window.getSize().x + 5, 0.f);
            }
            bounds[i].rotate(90); // Rotate for vertical alignment
        }
    }
}

// Renders the boundary sprites to the window.
static void DisplayBoundaries(sf::Sprite(&bounds)[4], sf::RenderWindow& window) {
    for (int i = 0; i < 4; i++) {
        window.draw(bounds[i]); // Draw each boundary sprite
    }
}
