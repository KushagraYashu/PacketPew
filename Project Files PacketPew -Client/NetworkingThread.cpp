// Author: Kushagra

#include "NetworkingThread.h"

using namespace std;

// Global variables
atomic<bool> isRunning(true);         // Flag to control the networking thread
atomic<bool> appClose(false);         // Flag to signal application closure
mutex networkMutex;                   // Mutex for thread synchronization
sf::TcpSocket socket;                 // TCP socket for communication
sf::Vector2f enemyPos;                // Current enemy position
sf::Vector2f lastEnemyPos;            // Last known enemy position
sf::Vector2f predictedPos;            // Predicted enemy position
sf::Vector2f enemyMoveDir;            // Enemy movement direction
float enemyRot = 0.f;                 // Current enemy rotation
float lastEnemyRot = 0.f;             // Last known enemy rotation
float currentTime = 0.f;              // Current timestamp
float serverTime = 0.f;               // Server timestamp
float lastTime = 0.f;                 // Last timestamp
float alpha = 0.f;                    // Interpolation alpha value
std::vector<sf::Packet> toSend;       // Packets queued to send to the server
std::queue<sf::Packet> serverPackets; // Packets received from the server
const int MAX_QUEUE_SIZE = 10;        // Maximum size of the server packet queue
int totLiveLost = 10;                 // Total allowed live check failures
int liveLost = 0;                     // Current live check failure count

// Send a live check packet to the server
void LiveCheck() {
    sf::Packet liveCheck;
    liveCheck << 1 << "SERVER_LIVE_CHECK";
    if (socket.send(liveCheck) != sf::Socket::Done) {
        cerr << "Error: Could not send SERVER_LIVE_CHECK packet." << endl;
        liveLost++;
    }
}

// Main networking thread function
void NetworkingThread() {
    while (isRunning) {
        // Periodically check if the server is alive
        LiveCheck();

        // Receive packets from the server
        sf::Packet packet;
        sf::Socket::Status stat = socket.receive(packet);
        if (stat == sf::Socket::Done) {
            // If a packet is received, add it to the server packets queue
            lock_guard<mutex> lock(networkMutex);
            serverPackets.push(packet);
        }
        else if (stat == sf::Socket::Disconnected) {
            // Handle server disconnection
            cerr << "Error: Server disconnected. Closing networking thread." << endl;
            isRunning = false;
            appClose = true;
            return;
        }

        // Send all queued packets to the server
        {
            lock_guard<mutex> lock(networkMutex);
            for (auto& packet : toSend) {
                socket.send(packet);
            }
            toSend.clear();
        }

        // Sleep briefly to control thread workload
        this_thread::sleep_for(chrono::milliseconds(5));
    }
}