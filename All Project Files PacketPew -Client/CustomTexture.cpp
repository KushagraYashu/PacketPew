// author: Kushagra

#pragma once

// Standard C++ headers
#include <iostream>

// SFML headers
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

// Custom headers
#include "CustomTexture.h"

// Default constructor for the Texture class
Texture::Texture() {}

// Create a texture by loading it from a file
bool Texture::CreateTexture(string texName, bool smooth, bool repeated) {
    // Construct the file path
    string texPath = "Textures/" + texName;

    // Attempt to load the texture from the specified file
    if (!m_tex.loadFromFile(texPath)) {
        return false; // Return false if loading failed
    }

    // Apply texture properties
    m_tex.setSmooth(smooth);
    m_tex.setRepeated(repeated);

    return true; // Return true if the texture was successfully loaded
}

// Accessor for the underlying SFML texture object
sf::Texture& Texture::getTex() {
    return m_tex;
}
