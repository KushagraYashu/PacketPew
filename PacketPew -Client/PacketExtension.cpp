//author: Kushagra

#include "PacketExtension.h"

sf::Packet& operator>>(sf::Packet& packet, sf::Vector2f& position) {
    float x, y;
    packet >> x >> y; // Extract x and y values
    position = sf::Vector2f(x, y); // Assign them to the sf::Vector2f
    return packet;
}

sf::Packet& operator<<(sf::Packet& packet, const sf::Vector2f& position) {
    return packet << position.x << position.y; // Insert x and y into the packet
}