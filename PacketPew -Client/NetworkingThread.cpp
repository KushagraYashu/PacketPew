//Author: Kushagra

#include "NetworkingThread.h"

using namespace std;

atomic<bool> isRunning(true);
atomic<bool> appClose(false);
mutex networkMutex;
sf::TcpSocket socket;
sf::Vector2f enemyPos;
sf::Vector2f lastEnemyPos;
float enemyRot;
float lastEnemyRot;
std::vector<sf::Packet> toSend;
std::queue<sf::Packet> serverPackets;
const int MAX_QUEUE_SIZE = 10;
int totLiveLost = 10;
int liveLost = 0;

void LiveCheck() {
	sf::Packet liveCheck;
	liveCheck << 1 << "SERVER_LIVE_CHECK";
	if (socket.send(liveCheck) != sf::Socket::Done) {
		cerr << "couldnt send SERVER_LIVE_CHECK\n";
		liveLost++;
	}
}

void NetworkingThread() {
	while (isRunning) {
		
		LiveCheck();

		sf::Packet packet;
		sf::Socket::Status stat = socket.receive(packet);
		if (stat == sf::Socket::Done) {
			lock_guard<mutex> lock(networkMutex);
			serverPackets.push(packet);
		}
		else if (stat == sf::Socket::Disconnected) {
			cerr << "closing networking thread since the server is offline\n";
			isRunning = false;
			appClose = true;
			return;
		}

		{
			lock_guard<mutex> lock(networkMutex);
			for (auto& packet : toSend) {
				socket.send(packet);
			}
			toSend.clear();
		}

		this_thread::sleep_for(chrono::milliseconds(5));
	}
}