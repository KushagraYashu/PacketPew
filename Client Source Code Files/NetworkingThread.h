// Author: Kushagra

#pragma once

// Required headers
#include <SFML/Network.hpp>
#include <thread>
#include <mutex>
#include <vector>
#include <atomic>
#include <queue>
#include <iostream>

// Global variables
extern std::atomic<bool> isRunning;       // Control flag to stop the networking thread
extern std::atomic<bool> appClose;        // Flag to signal application closure
extern std::mutex networkMutex;           // Mutex to synchronize access to shared resources
extern sf::TcpSocket socket;              // TCP socket for client-server communication

// Enemy-related variables
extern sf::Vector2f enemyPos;             // Current position of the enemy
extern sf::Vector2f lastEnemyPos;         // Last known position of the enemy
extern sf::Vector2f predictedPos;         // Predicted position of the enemy
extern float enemyRot;                    // Current rotation of the enemy
extern float lastEnemyRot;                // Last known rotation of the enemy
extern sf::Vector2f enemyMoveDir;         // Enemy's movement direction

// Time-related variables
extern float currentTime;                 // Current time for interpolation
extern float serverTime;                  // Time reported by the server
extern float lastTime;                    // Last recorded time
extern float alpha;                       // Interpolation alpha value for smoothing

// Packet management
extern std::vector<sf::Packet> toSend;    // Packets queued to be sent to the server
extern std::queue<sf::Packet> serverPackets; // Packets received from the server
extern const int MAX_QUEUE_SIZE;          // Maximum size for the server packets queue

// Server live check variables
extern int totLiveLost;                   // Maximum allowable live check failures
extern int liveLost;                      // Current count of live check failures

// Function declaration
void NetworkingThread();                  // Main networking thread function