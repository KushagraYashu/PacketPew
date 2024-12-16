//author: Kushagra

//cpp headers
#include <algorithm>
#include <iostream>

//sfml headers
#include<SFML/Network.hpp>
#include<SFML/Graphics.hpp>

//custom headers
#include "NetworkingThread.h"
#include "Player.h"
#include "Enemy.h"
#include "CustomTexture.h"
#include "PacketExtension.h"
#include "CustomWindow.cpp"
#include "CustomBoundaries.cpp"

using namespace std;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

//function declarations
void Quit();
void Won(string msg, string name);
int MainGame(string msg, string projectFullName);

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

    //connect to server
    sf::Socket::Status tcpConnectStat = socket.connect(serverIP, port);
    if (tcpConnectStat != sf::Socket::Done) {
        cerr << "couldnt connect to the server at " << serverIP.toString();
    }
    else {
        sf::Packet welcomePacket;
        if (socket.receive(welcomePacket) != sf::Socket::Done) {
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
                if (MainGame(msg, projectFullName) != 0) {
                    cerr << "some error in main game loop";
                }
            }
        }

    }

	return 0;
}

void Quit() {
    cout << "You Quit, game lost\n";
}

void Won(string msg, string name) {
    cout << "Enemy forfeited, you won!\n";
    cout << "Wanna try again? Y/N: ";
    char accept;
    cin >> accept;
    if (accept == 'Y' || accept == 'y') {
        MainGame(msg, name);
    }
    else {
        return;
    }
}

int MainGame(string msg, string projectFullName) {
    
        sf::Text clockText, helperText;
        sf::Font dotoFont, robotoFont;
        if (!dotoFont.loadFromFile("Fonts/Doto/Doto-Bold.ttf")) {
            std::cerr << "Error loading font file" << std::endl;
            return -1;
        }
        if (!robotoFont.loadFromFile("Fonts/Roboto/Roboto-Regular.ttf")) {
            std::cerr << "Error loading font file" << std::endl;
            return -1;
        }
        
        socket.setBlocking(false);
        sf::Packet playing;
        playing << 1 << "PLAYER_PLAYING";
        if (socket.send(playing) != sf::Socket::Done) {
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
        CreateGameWindow(window, projectFullName, false, 120, 8, icon);

        //setting up player related textures
        Texture playerTex, gunTex, bulletTex;
        playerTex.CreateTexture("checkerboardPlayer.png", true, false);
        gunTex.CreateTexture("guntex.png", true, false);
        bulletTex.CreateTexture("bullet.png", true, false);

        //creating player
        float moveRate = 4.0f;
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
        sf::Clock serverResponseClock;
        sf::Clock gameClock;
        sf::Time posTimeInterval = sf::milliseconds(50);

        //setting up text elements
        helperText.setPosition(sf::Vector2f());
        helperText.setFont(robotoFont);
        helperText.setCharacterSize(22);
        helperText.setFillColor(sf::Color::Black);
        helperText.setOutlineColor(sf::Color::White);
        helperText.setOutlineThickness(2);
        helperText.setStyle(sf::Text::Bold);
        helperText.setString("Q - Quit\nWASD - Move\nLMB - Shoot\nMouseX/Y - Look");
        clockText.setPosition(sf::Vector2f(25, window.getSize().y-50));
        clockText.setFont(dotoFont);
        clockText.setCharacterSize(20);
        clockText.setFillColor(sf::Color::Black);
        clockText.setOutlineColor(sf::Color::White);
        clockText.setOutlineThickness(1);
        clockText.setStyle(sf::Text::Italic | sf::Text::Bold);

        //network thread
        thread networkThread(NetworkingThread);

        //player actions
        struct PlayerActions {
            unsigned sequenceNo;
            sf::Vector2f moveDir;
        };

        unsigned curSequenceNo = 1;
        vector<PlayerActions> actions;

        bool enemyFire = false;

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
                    if (event.key.scancode == sf::Keyboard::Scan::Q) {
                        window.close();
                        Quit();
                    }
                    break;

                case sf::Event::MouseButtonPressed:
                    if (event.mouseButton.button == sf::Mouse::Left)
                    {
                        player.Fire(bulletTex.getTex());//Firing
                        //adding firing packet to the vector
                        sf::Packet playerFire;
                        playerFire << 1 << "PLAYER_FIRE" << player.GetPlayerSprite().getRotation();
                        {
                            lock_guard<mutex> lock(networkMutex);
                            toSend.push_back(playerFire);
                        }
                    }
                    break;
                }
            }

            //player related actions
            sf::Vector2f playerMovDir = player.CheckMove();//moving
            if (playerMovDir.x != 0 || playerMovDir.y != 0) {
                sf::Packet playerAct;
                playerAct << 1 << "PLAYER_ACTION_MOVE" << curSequenceNo << playerMovDir << player.GetMoveRate() << player.GetPlayerSprite().getPosition() << sf::Vector2f(player.GetMinX(), player.GetMinY()) << sf::Vector2f(player.GetMaxX(), player.GetMaxY()) << player.GetPlayerSprite().getRotation();
                {
                    lock_guard<mutex> lock(networkMutex);
                    toSend.push_back(playerAct);
                }
                cerr << "sending i want to move in :" << playerMovDir.x << "\t" << playerMovDir.y << endl;
                actions.push_back({ curSequenceNo, playerMovDir });
                curSequenceNo++;
                //moving the player, assuming the server will respond with the same position
                player.MovePredicted(playerMovDir);
            }



            //clear
            window.clear(sf::Color::Black);

            float deltaTime = clock.restart().asSeconds();

            /*if (networkClock.getElapsedTime() >= posTimeInterval) {
                sf::Packet playerPosRot;
                playerPosRot << 1 << "PLAYER_POS_ROT" << player.GetPlayerSprite().getPosition() << player.GetPlayerSprite().getRotation();
                {
                    lock_guard<mutex> lock(networkMutex);
                    toSend.push_back(playerPosRot);
                }
                networkClock.restart();
            }*/

            {
                lock_guard<mutex> lock(networkMutex);
                if (serverPackets.size() >= MAX_QUEUE_SIZE) {
                    cerr << "\t\t\t\t\t\tDropping 1\n";
                    serverPackets.pop();
                }
                while (!serverPackets.empty()) {
                    sf::Packet curPacket = serverPackets.front();
                    serverPackets.pop();

                    string type;
                    curPacket >> type;


                    if (type == "PLAYER_POS") {
                        int serverSequenceNo;
                        curPacket >> serverSequenceNo;
                        sf::Vector2f newPos;
                        curPacket >> newPos;
                        cerr << "receiving position as : " << newPos.x << "\t" << newPos.y << endl;

                        sf::Vector2f serverPlayerPos(newPos.x, newPos.y);
                        if (player.GetPlayerSprite().getPosition() != serverPlayerPos) {
                            player.GetPlayerSprite().setPosition(serverPlayerPos);
                            player.GetGunSprite().setPosition(player.GetPlayerSprite().getPosition());
                            for (const auto& action : actions) {
                                if (action.sequenceNo > serverSequenceNo) {
                                    player.MovePredicted(action.moveDir);
                                }
                            }

                            actions.erase(
                                std::remove_if(actions.begin(), actions.end(), [serverSequenceNo](const PlayerActions& action) {
                                    return action.sequenceNo == serverSequenceNo;
                                    }),
                                actions.end()
                            );
                            serverResponseClock.restart();
                        }
                    }

                    if (type == "ENEMY_POS_ROT") {
                        curPacket >> enemyMoveDir;
                        curPacket >> enemyPos;
                        curPacket >> enemyRot;
                        curPacket >> serverTime;
                        cout << "X: " << enemyPos.x << " Y: " << enemyPos.y << endl;
                        cout << "Rot: " << enemyRot << endl;
                        if (lastEnemyPos != enemyPos) {
                            lastEnemyPos = enemyPos;
                            /*predictedPos += (lastEnemyPos - predictedPos) * deltaTime;*/

                        }
                        else {
                            enemyMoveDir = sf::Vector2f();
                        }
                        if (lastEnemyRot != enemyRot) {
                            lastEnemyRot = enemyRot;
                        }

                    }

                    if (type == "ENEMY_FIRE") {
                        curPacket >> enemyRot;
                        if (lastEnemyRot != enemyRot) {
                            lastEnemyRot = enemyRot;
                        }
                        enemyFire = true;
                    }

                    if (type == "ENEMIES_NO") {
                        gameClock.restart();
                        int no;
                        curPacket >> no;
                        noEnemies = no;
                        enemy.SetAll(playerTex.getTex(), gunTex.getTex(), moveRate, window);
                        curPacket >> enemyRot;
                        if (stoi(msg) % 2 != 0) {
                            enemyPos = sf::Vector2f(SCREEN_WIDTH * 0.3f * (noEnemies + 1), SCREEN_HEIGHT * 0.5f);
                            lastEnemyPos = enemyPos;
                            predictedPos = enemyPos;
                        }
                        else {
                            enemyPos = sf::Vector2f(SCREEN_WIDTH * 0.3f * (noEnemies), SCREEN_HEIGHT * 0.5f);
                            lastEnemyPos = enemyPos;
                            predictedPos = enemyPos;
                        }

                    }

                    if (type == "ENEMY_DC") {
                        gameClock.restart();
                        int no;
                        curPacket >> no;
                        noEnemies = no;
                    }
                }
            }
            //draw
            clockText.setString(to_string(static_cast<int>(gameClock.getElapsedTime().asSeconds())));
            player.draw(window, deltaTime); //player and bullets
            if (noEnemies > 0) { //enemy logic 
                if (enemyFire) {
                    enemy.Fire(bulletTex.getTex(), lastEnemyRot);
                    enemyFire = false;
                }
                predictedPos += enemy.MovePredicted(enemyMoveDir);

                predictedPos += (lastEnemyPos - predictedPos) * (deltaTime * 4);
                enemy.draw(window, deltaTime, predictedPos, lastEnemyRot);
            }
            else if (noEnemies == -1) {
                window.close();
                /*isRunning = false;
                networkThread.join();*/
                Won(msg, projectFullName);
            }

            DisplayBoundaries(boundarySprites, window);//displaying boundaries

            if (appClose) {
                cerr << "closing game because server disconnected\n";
                window.close();
            }

            if (liveLost > totLiveLost) {
                cerr << "heavy SERVER_LIVE_CHECK failure, closing the application\n";
                window.close();
            }
            window.draw(clockText);
            window.draw(helperText);

            //render
            window.display();
        }

        isRunning = false;
        networkThread.join();
        return 0;
}