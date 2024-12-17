// author: Kushagra

// Texture header file

#pragma once

// Standard C++ headers
#include <iostream>

// SFML headers
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

using namespace std;

// A wrapper class for managing SFML textures
class Texture {
public:
    // Default constructor
    Texture();

    // Creates a texture by loading it from a file
    bool CreateTexture(string texName, bool smooth = false, bool repeated = true);

    // Returns a reference to the SFML texture
    sf::Texture& getTex();

private:
    sf::Texture m_tex; // The underlying SFML texture object
};
