//author: Kushagra

#pragma once

#include <SFML/Network.hpp>

sf::Packet& operator>>(sf::Packet& packet, sf::Vector2f& position);
sf::Packet& operator<<(sf::Packet& packet, const sf::Vector2f& position);