//author: Kushagra

#pragma once

//cpp headers
#include <iostream>

//sfml headers
#include<SFML/Network.hpp>
#include<SFML/Graphics.hpp>
#include <SFML/Window/Window.hpp>

static void CreateGameWindow(sf::RenderWindow& window, sf::VideoMode mode = sf::VideoMode(1280, 720), const sf::String& title = "", sf::Uint32 style = sf::Style::Default, const sf::ContextSettings& settings = sf::ContextSettings(), bool vsync = false, int antialiasingLevel = 0, sf::Image icon = sf::Image()) //create a window with defined size, title, style, settings, v-sync, anti-aliasing level, and an icon
{
	sf::ContextSettings newSet = settings;
	newSet.antialiasingLevel = antialiasingLevel;

	window.create(mode, title, style, settings);
	window.setVerticalSyncEnabled(vsync);

	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
}

static void CreateGameWindow(sf::RenderWindow& window, const sf::String& title = "", bool vsync = false, int maxFrameRate = 60, int antialiasingLevel = 0, sf::Image icon = sf::Image()) //create a window with defined title, v-sync, max fps, anti-aliasing level, and an icon
{
	sf::VideoMode mode = sf::VideoMode(1280, 720);
	sf::Uint32 style = sf::Style::Default;
	sf::ContextSettings settings = sf::ContextSettings();

	sf::ContextSettings newSet = settings;
	newSet.antialiasingLevel = antialiasingLevel;

	window.create(mode, title, style, settings);
	window.setVerticalSyncEnabled(vsync);
	window.setFramerateLimit(maxFrameRate);

	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
}