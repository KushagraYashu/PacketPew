//author: Kushagra

#pragma once

#include <SFML/Network.hpp>
#include <thread>
#include <mutex>
#include <vector>
#include <atomic>
#include <queue>
#include <iostream>

extern std::atomic<bool> isRunning;   // Control flag for the networking thread
extern std::atomic<bool> appClose;
extern std::mutex networkMutex;       // Mutex for protecting shared data
extern sf::TcpSocket socket;          // TCP socket for communication
extern sf::Vector2f enemyPos; // Shared enemy positions
extern sf::Vector2f lastEnemyPos;
extern sf::Vector2f predictedPos;
extern float enemyRot;
extern float lastEnemyRot;
extern sf::Vector2f enemyMoveDir;
extern float currentTime;
extern float serverTime;
extern float lastTime;
extern float alpha;
extern std::vector<sf::Packet> toSend;           // Shared outgoing packets
extern std::queue<sf::Packet> serverPackets;
extern const int MAX_QUEUE_SIZE;
extern int totLiveLost;
extern int liveLost;

void NetworkingThread();