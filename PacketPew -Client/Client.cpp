//author: Kushagra

//cpp headers
#include<iostream>

//sfml headers
#include<SFML/Network.hpp>
#include<SFML/Graphics.hpp>

//custom headers
#include "Player.h"
#include "Enemy.h"
#include "CustomTexture.h"
#include "PacketExtension.h"
#include "CustomWindow.cpp"
#include "CustomBoundaries.cpp"

using namespace std;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

int main() {
	//debug msg (ignore)
	cout << "hello from client\n";

    //setting project name and version
    string versionNo = "1.0";
    string projectFullName = "PacketPew Client ";
    projectFullName.append(versionNo);

    //networking things
    int port = 6969;
    sf::IpAddress serverIP = sf::IpAddress("127.0.0.1"); //TODO: change this so you can input a string from the user to connect to the server
    sf::TcpSocket client;

    //connect to server
    sf::Socket::Status tcpConnectStat = client.connect(serverIP, port);
    if (tcpConnectStat != sf::Socket::Done) {
        cerr << "couldnt connect to the server at " << serverIP.toString();
    }
    else {
        sf::Packet welcomePacket;
        if (client.receive(welcomePacket) != sf::Socket::Done) {
            cerr << "something's wrong with welcome msg";
            return -1;
        }
        string msg;
        welcomePacket >> msg;
        if (msg == "-1") {
            cerr << "Server cant accommodate you right now, sorry\n";
            return 0;
        }
        else {
            cout << "Welcome to \"Packet Pew\"\nPress \"Y\" for playing: ";
            char accept;
            cin >> accept;
            if (accept == 'Y' || accept == 'y') {
                
                client.setBlocking(false);
                sf::Packet playing;
                playing << 1 << "PLAYER_PLAYING";
                if (client.send(playing) != sf::Socket::Done) {
                    cerr << "couldn't send PLAYER_PLAYING\n";
                    return -1;
                }

                sf::Vector2f iniPosition = sf::Vector2f(SCREEN_WIDTH * 0.3f * stoi(msg), SCREEN_HEIGHT * 0.5f);

                //creating an icon
                sf::Image icon;
                if (!icon.loadFromFile("Icon/demo.png")) {
                    std::cerr << "Error loading icon file" << std::endl;
                    return -1;
                }

                //creating a window
                sf::RenderWindow window;
                CreateWindow(window, projectFullName, true, 8, icon);

                //setting up player related textures
                Texture playerTex, gunTex, bulletTex;
                playerTex.CreateTexture("checkerboardPlayer.png", true, false);
                gunTex.CreateTexture("guntex.png", true, false);
                bulletTex.CreateTexture("bullet.png", true, false);

                //creating player
                float moveRate = 1.0f;
                Player player(playerTex.getTex(), gunTex.getTex(), moveRate, window);
                player.SetPlayerPosition(iniPosition);
                int noEnemies = 0;
                Enemy enemy;

                //setting up boundaries
                Texture boundTex;
                boundTex.CreateTexture("brickLine.png", true);
                sf::Sprite boundarySprites[4];
                CreateBoundaries(boundarySprites, boundTex.getTex(), window);

                //clock
                sf::Clock clock;
                sf::Clock networkClock;
                sf::Time posTimeInterval = sf::milliseconds(50);
                sf::Vector2f lastPos = sf::Vector2f();
                float lastRot = 0;
                float enemyRot = 0;
                sf::Vector2f enemyPos;

                //GAME LOOP
                while (window.isOpen()) {
                    
                    sf::Packet demo;
                    demo << 1 << "SERVER_LIVE_CHECK";
                    if (client.send(demo) != sf::Socket::Done) {
                        cerr << "couldn't send SERVER_LIVE_CHECK\n";
                        window.close();
                        return -1;
                    }

                    //handling events
                    sf::Event event;
                    while (window.pollEvent(event)) {
                        switch (event.type) {
                        case sf::Event::Closed: //closing
                            window.close();
                            break;

                        case sf::Event::KeyReleased:
                            if (event.key.scancode == sf::Keyboard::Scan::W) {
                                player.MoveUp(false);
                            }
                            if (event.key.scancode == sf::Keyboard::Scan::A) {
                                player.MoveLeft(false);
                            }
                            if (event.key.scancode == sf::Keyboard::Scan::S) {
                                player.MoveDown(false);
                            }
                            if (event.key.scancode == sf::Keyboard::Scan::D) {
                                player.MoveRight(false);
                            }
                            break;

                        case sf::Event::KeyPressed:
                            if (event.key.scancode == sf::Keyboard::Scan::W) {
                                player.MoveUp(true);
                            }
                            if (event.key.scancode == sf::Keyboard::Scan::A) {
                                player.MoveLeft(true);
                            }
                            if (event.key.scancode == sf::Keyboard::Scan::S) {
                                player.MoveDown(true);
                            }
                            if (event.key.scancode == sf::Keyboard::Scan::D) {
                                player.MoveRight(true);
                            }
                            break;

                        case sf::Event::MouseButtonPressed:
                            if (event.mouseButton.button == sf::Mouse::Left)
                            {
                                player.Fire(bulletTex.getTex());//Firing
                            }
                            break;
                        }
                    }

                    //player related actions
                    player.CheckMove();//moving

                    //clear
                    window.clear(sf::Color::Black);

                    float deltaTime = clock.restart().asSeconds();

                    if (networkClock.getElapsedTime() >= posTimeInterval) {
                        sf::Packet playerPosRot;
                        playerPosRot << 1 << "PLAYER_POS_ROT" << player.GetPlayerSprite().getPosition() << player.GetPlayerSprite().getRotation();
                        if (client.send(playerPosRot) != sf::Socket::Done) {
                            cerr << "couldn't send ENEMY_POS_ROT\n";
                            window.close();
                            return -1;
                        }
                        networkClock.restart();
                    }

                    sf::Packet serverPacket;
                    string type;

                    sf::Socket::Status stat = client.receive(serverPacket);
                    if (stat == sf::Socket::NotReady) {
                        //not ready
                    }
                    else if (stat == sf::Socket::Disconnected) {
                        cerr << "couldn't receive from server, server is offline\n";
                        window.close();
                        return -1;
                    }
                    else if (stat != sf::Socket::Done) {
                        cerr << "something bad happend, couldnt receive from server\n";
                        window.close();
                        return -1;
                    }
                    serverPacket >> type;

                    if (type == "ENEMIES_NO") {
                        int no;
                        serverPacket >> no;
                        noEnemies = no;
                        enemy.SetAll(playerTex.getTex(), gunTex.getTex(), moveRate, window);
                        serverPacket >> enemyRot;
                        if (stoi(msg) % 2 != 0) {
                            enemyPos = sf::Vector2f(SCREEN_WIDTH * 0.3f * (noEnemies + 1), SCREEN_HEIGHT * 0.5f);
                        }
                        else {
                            enemyPos = sf::Vector2f(SCREEN_WIDTH * 0.3f * (noEnemies), SCREEN_HEIGHT * 0.5f);
                        }

                    }

                    if (type == "ENEMY_POS_ROT") {
                        serverPacket >> enemyPos;
                        serverPacket >> enemyRot;
                        cout << "X: " << enemyPos.x << " Y: " << enemyPos.y << endl;
                        cout << "Rot: " << enemyRot << endl;
                    }

                    if (lastPos != enemyPos) { 
                        lastPos = enemyPos;
                    }
                    if (lastRot != enemyRot) {
                        lastRot = enemyRot;
                    }
                    
                    //draw
                    player.draw(window, deltaTime); //player and bullets
                    if (noEnemies != 0) { //enemy logic
                        enemy.draw(window, deltaTime, lastPos, lastRot);
                    }

                    DisplayBoundaries(boundarySprites, window);//displaying boundaries

                    //render
                    window.display();
                }
            }
            else {
                return 0;
            }
        }

    }

	return 0;
}