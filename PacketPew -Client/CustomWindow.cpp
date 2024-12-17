// author: Kushagra

#pragma once

// Standard C++ headers
#include <iostream>

// SFML headers
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Window.hpp>

// Creates a game window with specified parameters.

static void CreateGameWindow(
    sf::RenderWindow& window,
    sf::VideoMode mode = sf::VideoMode(1280, 720),
    const sf::String& title = "",
    sf::Uint32 style = sf::Style::Default,
    const sf::ContextSettings& settings = sf::ContextSettings(),
    bool vsync = false,
    int antialiasingLevel = 0,
    sf::Image icon = sf::Image()
) {
    sf::ContextSettings newSettings = settings;
    newSettings.antialiasingLevel = antialiasingLevel;

    window.create(mode, title, style, newSettings);
    window.setVerticalSyncEnabled(vsync);

    if (!icon.getSize().x || !icon.getSize().y) return; // Ensure icon has valid size
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
}

// Overloaded version to create a game window with simplified parameters.
static void CreateGameWindow(
    sf::RenderWindow& window,
    const sf::String& title = "",
    bool vsync = false,
    int maxFrameRate = 60,
    int antialiasingLevel = 0,
    sf::Image icon = sf::Image()
) {
    sf::VideoMode mode = sf::VideoMode(1280, 720);
    sf::Uint32 style = sf::Style::Default;
    sf::ContextSettings settings;

    settings.antialiasingLevel = antialiasingLevel;

    window.create(mode, title, style, settings);
    window.setVerticalSyncEnabled(vsync);
    window.setFramerateLimit(maxFrameRate);

    if (!icon.getSize().x || !icon.getSize().y) return; // Ensure icon has valid size
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
}
