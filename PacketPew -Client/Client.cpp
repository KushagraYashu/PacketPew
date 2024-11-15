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

    //creating an icon
    sf::Image icon;
    if (!icon.loadFromFile("Icon/demo.png")) {
        std::cerr << "Error loading icon file" << std::endl;
        return -1;
    }

    //creating a window
    sf::RenderWindow window;
    CreateWindow(window, projectFullName, true, 8, icon);

    //setting up player texture
    Texture playerTex, gunTex;
    playerTex.CreateTexture("checkerboardPlayer.png", true, false);
    gunTex.CreateTexture("guntex.png", true, false);

    //creating player
    float moveRate = 1.0f;
    Player player(playerTex.getTex(), gunTex.getTex(), moveRate, window);

    //setting up boundaries
    Texture boundTex;
    boundTex.CreateTexture("brickLine.png", true);
    sf::Sprite boundarySprites[4];
    CreateBoundaries(boundarySprites, boundTex.getTex(), window);

    //GAME LOOP
    while (window.isOpen()) {

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
            }
        }

        //player related actions
        player.CheckMove();//moving

        //clear
        window.clear(sf::Color::Black);

        //draw
       player.draw(window); //player

        DisplayBoundaries(boundarySprites, window);//displaying boundaries

        //render
        window.display();
    }

	return 0;
}