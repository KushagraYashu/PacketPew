//author: Kushagra

#include "PacketExtension.h"

sf::Packet& operator>>(sf::Packet& packet, sf::Vector2f& position) {
    float x, y;
    packet >> x >> y; 
    position = sf::Vector2f(x, y);
    return packet;
}

sf::Packet& operator<<(sf::Packet& packet, const sf::Vector2f& position) {
    return packet << position.x << position.y;
}