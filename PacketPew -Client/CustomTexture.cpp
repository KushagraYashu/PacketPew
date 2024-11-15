//author: Kushagra

#pragma once

//cpp headers
#include <iostream>

//sfml headers
#include<SFML/Network.hpp>
#include<SFML/Graphics.hpp>

//custom headers
#include "CustomTexture.h"

using namespace std;

Texture::Texture() {
	
}

bool Texture::CreateTexture(string texName, bool smooth, bool repeated) {
	string texPath = "Textures/";
	texPath.append(texName);
	if (!m_tex.loadFromFile(texPath)) {
		return false;
	}
	m_tex.setSmooth(smooth);
	m_tex.setRepeated(repeated);
	return true;
}

sf::Texture& Texture::getTex() {
	return m_tex;
}