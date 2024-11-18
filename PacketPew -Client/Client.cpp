//author: Kushagra

//cpp headers
#include<iostream>

//sfml headers
#include<SFML/Network.hpp>
#include<SFML/Graphics.hpp>

//custom headers
#include "Player.h"
#include "CustomWindow.cpp"
#include "CustomTexture.h"
#include "CustomBoundaries.cpp"

using namespace std;

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
        client.receive(welcomePacket);
        string msg;
        welcomePacket >> msg;
        if (msg == "-1") {
            cerr << "Server cant accommodate you right now, sorry\n";
            return 0;
        }
        else if (msg == "1") {
            cout << "Welcome to \"Packet Pew\"\nPress \"Y\" for playing: ";
            char accept;
            cin >> accept;
            if (accept == 'Y' || accept == 'y') {
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

                //setting up boundaries
                Texture boundTex;
                boundTex.CreateTexture("brickLine.png", true);
                sf::Sprite boundarySprites[4];
                CreateBoundaries(boundarySprites, boundTex.getTex(), window);

                //clock
                sf::Clock clock;

                //GAME LOOP
                while (window.isOpen()) {
                    sf::Packet demo;
                    if (client.send(demo) != sf::Socket::Done) {
                        window.close();
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

                    //draw
                    player.draw(window, clock.restart().asSeconds()); //player and bullets

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