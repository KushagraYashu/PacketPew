//author: Kushagra

//Texture header file

#pragma once

//cpp headers
#include<iostream>

//sfml headers
#include<SFML/Network.hpp>
#include<SFML/Graphics.hpp>

using namespace std;

class Texture {
public: 
	Texture();

	bool CreateTexture(string texName, bool smooth = false, bool repeated = true);

	sf::Texture& getTex();

private:
	sf::Texture m_tex;
};