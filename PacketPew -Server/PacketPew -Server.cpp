//author: Kushagra

//cpp headers
#include <iostream>
#include <list>

//sfml headers
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

//custom headers
#include "PacketExtension.h"

using namespace std;

const int SELECTOR_WAIT_TIME_MS = 200;
const int MAX_CLIENTS = 2;
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

struct ClientData {
    sf::TcpSocket* socket;
    unsigned sequenceNo;
    sf::Vector2f position;
    sf::Sprite playerSprite;
    float rotation;
    string id;
};
ClientData clientDataArray[2];
int index = 0;

struct ClientInputs {
    sf::TcpSocket* socket;
    unsigned sequenceNo;
    sf::Vector2f moveDir;
    float moveRate;
    sf::Vector2f curPos;
    sf::Vector2f newPos;
    sf::Vector2f min, max;
    float rot;
};
list<ClientInputs> clientInputsList;

struct Bullet {
public:
    sf::Sprite bulletSprite;
    sf::Vector2f pos;
    std::string id;
    sf::Vector2f m_velocity;
    float m_speed;
    float m_lifetime;
};
vector<Bullet> bullets;

void BroadcastToAll(list<sf::TcpSocket*>& clients, sf::TcpSocket& except, sf::Packet& sendPacket, sf::SocketSelector& selector);
void BroadcastToAll(list<sf::TcpSocket*>& clients, sf::Packet& sendPacket, sf::SocketSelector& selector);

void EnemyDC(list<sf::TcpSocket*>& clients, sf::SocketSelector& selector) {
    sf::Packet enemyDisconnected;
    enemyDisconnected << "ENEMY_DC" << -1;
    BroadcastToAll(clients, enemyDisconnected, selector);
}

void BroadcastToAll(list<sf::TcpSocket*>& clients, sf::TcpSocket& except, sf::Packet& sendPacket, sf::SocketSelector& selector) {
    bool dc = false;
    for (auto it = clients.begin(); it != clients.end(); ) {
        sf::TcpSocket& sendClient = **it;
        if (&sendClient == &except) {
            cout << "skipping " << sendClient.getRemotePort() << endl;
            ++it;
            continue;
        }
        sf::Socket::Status tcpSendStat = sendClient.send(sendPacket);

        if (tcpSendStat == sf::Socket::Disconnected) {
            cerr << "A client disconnected. Removing from the list.\n";
            dc = true;
            selector.remove(sendClient);
            sendClient.disconnect();
            it = clients.erase(it);
        }
        else {
            ++it;
        }
    }
    if (dc) {
        EnemyDC(clients, selector);
    }
}

void BroadcastToAll(list<sf::TcpSocket*>& clients, sf::Packet& sendPacket, sf::SocketSelector& selector) {
    bool dc = false;
    for (auto it = clients.begin(); it != clients.end(); ) {
        sf::TcpSocket& sendClient = **it;
        sf::Socket::Status tcpSendStat = sendClient.send(sendPacket);

        if (tcpSendStat == sf::Socket::Disconnected) {
            cerr << "A client disconnected. Removing from the list.\n";
            dc = true;
            selector.remove(sendClient);
            sendClient.disconnect();
            it = clients.erase(it);
        }
        else {
            ++it;
        }
    }
    if (dc) {
        EnemyDC(clients, selector);
    }
}

sf::Packet RecieveFromClient(sf::TcpSocket& client, list<sf::TcpSocket*>& clients, sf::SocketSelector& selector) {
    sf::Packet clientDataPacket;
    sf::Socket::Status tcpRecvStat = client.receive(clientDataPacket);
    if (tcpRecvStat == sf::Socket::Disconnected) {
        //error...
        cerr << "A client disconnected\n";
        clients.remove(&client);
        selector.remove(client);
        client.disconnect();
        printf("waiting for a new connection\n");
        EnemyDC(clients, selector);
        sf::Packet returnPacket;
        returnPacket << -1;
        return returnPacket;
    }
    if (tcpRecvStat == sf::Socket::NotReady) {
        sf::Packet returnPacket;
        returnPacket << 0;
        return returnPacket;
    }
    
    return clientDataPacket;
}

void AddClients(sf::TcpListener& serverListener, list<sf::TcpSocket*>& clients, sf::SocketSelector& selector) {
    auto* client = new sf::TcpSocket();
    sf::Packet welcomePacket;
    string msg;
    if (serverListener.accept(*client) == sf::Socket::Done) {
        if (clients.size() < MAX_CLIENTS) {
            msg = to_string(clients.size() + 1);
            welcomePacket << msg;
            if (client->send(welcomePacket) == sf::Socket::Done) {
                printf("A client connected at port %d and address %s\n", client->getRemotePort(), client->getRemoteAddress().toString().data());
                clients.push_back(client);
                clientDataArray[index % 2].socket = client;
                clientDataArray[index % 2].id = "player" + to_string((index % 2) + 1);
                index++;
                std::cout << "Total clients connected " << clients.size() << std::endl;
                selector.add(*client);
            }
        }
        else {
            msg = "-1";
            welcomePacket << msg;
            client->send(welcomePacket);
            client->disconnect();
            delete client;
        }
    }
    else {
        delete client;
    }
}

void BroadcastEnemies(list<sf::TcpSocket*>& clients, sf::SocketSelector& selector) {
    sf::Packet enemiesData;
    int enemies = clients.size() - 1;
    enemiesData << "ENEMIES_NO" << enemies << 0;
    BroadcastToAll(clients, enemiesData, selector);
}

sf::Vector2f PerformMove(sf::Vector2f moveDir, float moveRate, sf::Vector2f curPlayerPos, sf::Vector2f min, sf::Vector2f max) {
    int dx = (moveDir.x != 0) ? moveDir.x / abs(moveDir.x) : 0;
    int dy = (moveDir.y != 0) ? moveDir.y / abs(moveDir.y) : 0;
    float length = sqrt((dx * dx) + (dy * dy));
    sf::Vector2f direction = sf::Vector2f(dx / length, dy / length);
    sf::Vector2f pos = curPlayerPos + sf::Vector2f(direction.x * moveRate, direction.y * moveRate);
    float clampedPosX = std::clamp(pos.x, min.x, max.x);
    float clampedPosY = std::clamp(pos.y, min.y, max.y);
    return sf::Vector2f(clampedPosX, clampedPosY);
}

void CreateBullet(std::vector<Bullet>& bullets, sf::Texture bulletTex, sf::Vector2f spawnPos, std::string id, float spawnRot, float speed, float lifetime) {
    Bullet bullet;
    bullet.bulletSprite.setTexture(bulletTex);
    bullet.bulletSprite.setOrigin(bullet.bulletSprite.getLocalBounds().width / 2, bullet.bulletSprite.getLocalBounds().height / 2);
    bullet.bulletSprite.setPosition(spawnPos);
    bullet.m_speed = speed;
    bullet.id = id;
    bullet.pos = spawnPos;
    float angleRad = spawnRot * 3.14159f / 180.f;
    bullet.m_velocity = sf::Vector2f(std::cos(angleRad) * bullet.m_speed, std::sin(angleRad) * bullet.m_speed);
    bullet.m_lifetime = lifetime;
    bullets.push_back(bullet);
}

void CheckCollision(std::list<sf::TcpSocket*> clients, sf::SocketSelector selector) {
    bool hit = false;
    for (auto it = bullets.begin(); it != bullets.end();) {
        sf::FloatRect bulletBounds = it->bulletSprite.getGlobalBounds();
        for (auto& client : clientDataArray) {
            if (client.position != sf::Vector2f() && bulletBounds.intersects(client.playerSprite.getGlobalBounds()) && it->id != client.id) {
                cout << it->id << " bullet hit " << client.id << endl;
                hit = true;
                sf::Packet hitPacket;
                hitPacket << "BULLET_HIT" << it->id << client.id;
                it = bullets.erase(it);
                BroadcastToAll(clients, hitPacket, selector);
                break;
            }
        }
        if (hit) {
            hit = false;
            continue;
        }
        else {
            it++;
        }
    }
}

void UpdateBullets(std::list<sf::TcpSocket*> clients, sf::SocketSelector selector, float deltaTime) {
    CheckCollision(clients, selector);
    for (auto it = bullets.begin(); it != bullets.end();) {
        it->bulletSprite.move(it->m_velocity * deltaTime);
        it->pos = it->bulletSprite.getPosition();
        it->m_lifetime -= deltaTime;
        if (it->m_lifetime <= 0) {
            it = bullets.erase(it);
        }
        else {
            ++it;
        }
    }
}

int main()
{
    //setting project name and version
    string versionNo = "1.0";
    string projectFullName = "PacketPew Server ";
    projectFullName.append(versionNo);

    //setting textures
    sf::Texture playerTex, bulletTex;
    playerTex.loadFromFile("Textures/checkerboardPlayer.png");
    bulletTex.loadFromFile("Textures/bullet.png");
    clientDataArray[0].playerSprite.setTexture(playerTex);
    clientDataArray[0].playerSprite.setOrigin(clientDataArray[0].playerSprite.getLocalBounds().width / 2, clientDataArray[0].playerSprite.getLocalBounds().height / 2);
    clientDataArray[1].playerSprite.setTexture(playerTex);
    clientDataArray[1].playerSprite.setOrigin(clientDataArray[1].playerSprite.getLocalBounds().width / 2, clientDataArray[1].playerSprite.getLocalBounds().height / 2);

    //setting server variables
    int port = 54000;
    sf::IpAddress serverIP = sf::IpAddress("127.0.0.1"); //TODO: change this in a way that you can connect to the same machines on the network
    sf::TcpListener serverListener;

    char choice;
    cout << "Do you want to use localhost (127.0.0.1)? Y/N: ";
    cin >> choice;
    if (choice != 'Y' && choice != 'y') {
        cout << "Enter the ip you want to assign as server: ";
        string ip;
        cin >> ip;
        serverIP = sf::IpAddress(ip);
    }

    //clients
    std::list<sf::TcpSocket*> clients;
    int clientPosMsgIndex = 0;

    //selector (for non blocking)
    sf::SocketSelector selector;

    // bind the server to a port
    if (serverListener.listen(port, serverIP) != sf::Socket::Done)
    {
        // error...
        cerr << "\nerror in binding the server port";
    }
    else {
        std::string addr = serverIP.toString();
        printf("\nlistening at port %d and address %s\n", port, addr.data());
        selector.add(serverListener);
    }

    //clock and broadcast time
    sf::Clock broadcastClock;
    sf::Clock serverClock;
    sf::Clock bulletClock;
    const sf::Time broadcastTime = sf::milliseconds(50); //20 updates per second

    while (true) {
        if (selector.wait(sf::milliseconds(SELECTOR_WAIT_TIME_MS))) {
            //handling new connections
            if (selector.isReady(serverListener)) {
                AddClients(serverListener, clients, selector);
                clientInputsList.clear();
                serverClock.restart();
            }
            else {
                float deltaTime = bulletClock.restart().asSeconds();
                UpdateBullets(clients, selector, deltaTime);
                list<sf::TcpSocket*> clientsCopy = clients;
                for (std::list<sf::TcpSocket*>::iterator it = clientsCopy.begin(); it != clientsCopy.end(); ++it) {
                    sf::TcpSocket& client = **it;
                    if (selector.isReady(client)) {
                        sf::Packet msg = RecieveFromClient(client, clients, selector);
                        int stat;
                        msg >> stat;
                        if (stat == -1) {
                            cerr << "error in recieving from a client\n";
                        }
                        else if (stat == 0) {
                            //not ready
                        }
                        else if (stat == 1){
                            string type;
                            msg >> type;
                            if (type == "PLAYER_PLAYING") {
                                cout << "relay enemy joined\n";
                                BroadcastEnemies(clients, selector);
                            }
                            if (type == "SERVER_LIVE_CHECK") {
                                //server live check
                            }
                            if (type == "PLAYER_ACTION_MOVE") {
                                unsigned sequenceNo;
                                msg >> sequenceNo;
                                sf::Vector2f moveDir;
                                msg >> moveDir;
                                float moveRate;
                                msg >> moveRate;
                                sf::Vector2f curPlayerPos;
                                msg >> curPlayerPos;
                                sf::Vector2f min, max;
                                msg >> min;
                                msg >> max;
                                float playerRot;
                                msg >> playerRot;
                                for (auto& clientData : clientDataArray) {
                                    if (&client == clientData.socket) {
                                        clientData.position = curPlayerPos;
                                        clientData.playerSprite.setPosition(curPlayerPos);
                                    }
                                }
                                clientInputsList.push_back({ &client, sequenceNo, moveDir, moveRate, curPlayerPos, sf::Vector2f(), min, max, playerRot });
                                /*sf::Vector2f playerPosNew = PerformMove(moveDir, moveRate, curPlayerPos, min, max);
                                sf::Packet playerPosPacket;
                                playerPosPacket << "PLAYER_POS" << sequenceNo << playerPosNew;
                                if (client.send(playerPosPacket) != sf::Socket::Done) {
                                    cerr << "error in sending the calculated position";
                                }
                                sf::Packet enemyDataPacket;
                                enemyDataPacket << "ENEMY_POS_ROT" << playerPosNew << playerRot;
                                BroadcastToAll(clients, client, enemyDataPacket, selector);*/
                            }
                            /*if (type == "PLAYER_POS_ROT") {
                                if (broadcastClock.getElapsedTime() >= broadcastTime) {
                                    ClientData clientData;
                                    msg >> clientData.position;
                                    msg >> clientData.rotation;
                                    clientData.socket = &client;
                                    clientData.index = clientPosMsgIndex;
                                    clientPosMsgIndex++;
                                    clientDataList.push_back(clientData);
                                    sf::Packet enemyPos;
                                    enemyPos << "ENEMY_POS_ROT" << clientData.position << clientData.rotation;
                                    BroadcastToAll(clients, client, enemyPos, selector);
                                    broadcastClock.restart();
                                }
                            }*/
                            if (type == "PLAYER_FIRE") {
                                sf::Vector2f pos;
                                float playerRot;
                                string id;
                                float speed;
                                float lifetime;
                                msg >> pos;
                                msg >> playerRot;
                                msg >> id;
                                msg >> speed;
                                msg >> lifetime;
                                for (auto& clientData : clientDataArray) {
                                    if (&client == clientData.socket) {
                                        clientData.id = id;
                                        clientData.position = pos;
                                        clientData.playerSprite.setPosition(pos);
                                        clientData.rotation = playerRot;
                                        clientData.playerSprite.setRotation(playerRot);
                                    }
                                }
                                CreateBullet(bullets, bulletTex, pos, id, playerRot, speed, lifetime);
                                sf::Packet enemyFire;
                                enemyFire << "ENEMY_FIRE" << playerRot;
                                BroadcastToAll(clients, client, enemyFire, selector);
                            }
                        }
                        
                    }
                }
                if (broadcastClock.getElapsedTime() >= broadcastTime) {
                    if (!clientInputsList.empty()) {
                        for (auto &input : clientInputsList) {
                            input.newPos = PerformMove(input.moveDir, input.moveRate, input.curPos, input.min, input.max);
                            for (auto& client : clientDataArray) {
                                if (input.socket == client.socket) {
                                    if (input.sequenceNo > client.sequenceNo) {
                                        client.position = input.newPos;
                                        client.playerSprite.setPosition(input.newPos);
                                        client.rotation = input.rot;
                                        client.playerSprite.setRotation(input.rot);
                                        client.rotation = input.rot;
                                        client.sequenceNo = input.sequenceNo;
                                    }
                                }
                            }
                        }
                        
                        sf::Packet playerPosPacket;
                        sf::TcpSocket& client = *clientInputsList.back().socket;
                        playerPosPacket << "PLAYER_POS" << clientInputsList.back().sequenceNo << clientInputsList.back().newPos;
                        auto stat = client.send(playerPosPacket);
                        if (stat == sf::Socket::Disconnected) {
                            cerr << "client Disconnected\n";
                            EnemyDC(clients, selector);
                        }
                        else if (stat != sf::Socket::Done) {
                            cerr << "some error in sending positions\n";

                        }
                        sf::Packet enemyDataPacket;
                        float time = serverClock.getElapsedTime().asMilliseconds();
                        enemyDataPacket << "ENEMY_POS_ROT" << clientInputsList.back().moveDir << clientInputsList.back().newPos << clientInputsList.back().rot << time;
                        BroadcastToAll(clients, client, enemyDataPacket, selector);
                    }
                    broadcastClock.restart();
                }
                
            }
        }
        
    }

    return 0;
}
