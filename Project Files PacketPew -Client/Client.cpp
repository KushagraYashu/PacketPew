//author: Kushagra

// Standard C++ headers
#include <algorithm>
#include <iostream>

// SFML headers
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

// Custom headers
#include "NetworkingThread.h"
#include "Player.h"
#include "Enemy.h"
#include "CustomTexture.h"
#include "PacketExtension.h"
#include "CustomWindow.cpp"
#include "CustomBoundaries.cpp"

using namespace std;

// Screen dimensions
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

// Function declarations
void Quit();
void Lost();
void WonForfeit();
void WonKill();
int MainGame(string msg, string projectFullName);

int main() {
    // Set project name and version
    string versionNo = "1.0";
    string projectFullName = "PacketPew Client " + versionNo;

    // Networking variables
    int port = 54000;
    sf::IpAddress serverIP = sf::IpAddress("127.0.0.1");

    char choice;
    cout << "Do you want to use localhost (127.0.0.1)? Y/N: ";
    cin >> choice;
    if (choice != 'Y' && choice != 'y') {
        cout << "Enter the server IP: ";
        string ip;
        cin >> ip;
        serverIP = sf::IpAddress(ip);
    }

    bool restartGame = false;

    do {
        restartGame = false; // Reset restart flag

        {
            lock_guard<mutex> lock(networkMutex);
            liveLost = 0; // Reset live lost variable
        }

        // Attempt to connect to the server
        sf::Socket::Status tcpConnectStat = socket.connect(serverIP, port);
        if (tcpConnectStat != sf::Socket::Done) {
            cerr << "Error: Couldn't connect to the server at " << serverIP.toString() << endl;
            break; // Exit if connection fails
        }

        sf::Packet welcomePacket;
        if (socket.receive(welcomePacket) != sf::Socket::Done) {
            cerr << "Error: Couldn't receive the welcome message." << endl;
            break; // Exit if welcome packet fails
        }

        string msg;
        welcomePacket >> msg;
        if (msg == "-1") {
            cerr << "Error: Server can't accommodate you right now." << endl;
            break;
        }

        cout << "Welcome to \"Packet Pew\"\nPress \"Y\" to play: ";
        char accept;
        cin >> accept;

        if (accept == 'Y' || accept == 'y') {
            isRunning = true;
            if (MainGame(msg, projectFullName) != 0) {
                cerr << "Error: Issue in main game loop." << endl;
            }

            // Ask for restart after the game ends
            cout << "Do you want to play again? Y/N: ";
            cin >> accept;
            if (accept == 'Y' || accept == 'y') {
                restartGame = true; // Restart the game
                socket.disconnect(); // Disconnect before restarting
                socket.setBlocking(true);
            }
        }
    } while (restartGame);

    cout << "Exiting game. Goodbye!" << endl;
    return 0;
}

// Handle quitting the game
void Quit() {
    isRunning = false;
    socket.setBlocking(true);
    socket.disconnect();
    cout << "You Quit. Game lost.\nReturning to main menu..." << endl;
}

// Handle player losing the game
void Lost() {
    isRunning = false;
    socket.setBlocking(true);
    socket.disconnect();
    cout << "You Died! You Lost!\nReturning to main menu..." << endl;
}

// Handle enemy forfeiting the game
void WonForfeit() {
    isRunning = false;
    socket.setBlocking(true);
    socket.disconnect();
    cout << "Enemy forfeited. You Won!\nReturning to main menu..." << endl;
}

// Handle player killing the enemy
void WonKill() {
    isRunning = false;
    socket.setBlocking(true);
    socket.disconnect();
    cout << "You killed the enemy. You Won!\nReturning to main menu..." << endl;
}

int MainGame(string msg, string projectFullName) {
    
    sf::Text clockText, helperText, playerHealth, enemyHealth;
    sf::Font dotoFont, robotoFont;

    // Load fonts
    if (!dotoFont.loadFromFile("Fonts/Doto/Doto-Bold.ttf")) {
        cerr << "Error: Failed to load Doto-Bold.ttf font." << endl;
        return -1;
    }
    if (!robotoFont.loadFromFile("Fonts/Roboto/Roboto-Regular.ttf")) {
        cerr << "Error: Failed to load Roboto-Regular.ttf font." << endl;
        return -1;
    }

    // Set up networking
    socket.setBlocking(false);
    sf::Packet playing;
    playing << 1 << "PLAYER_PLAYING";
    if (socket.send(playing) != sf::Socket::Done) {
        cerr << "Error: Couldn't send PLAYER_PLAYING packet." << endl;
        return -1;
    }

    sf::Vector2f iniPosition = sf::Vector2f(SCREEN_WIDTH * 0.3f * stoi(msg), SCREEN_HEIGHT * 0.5f);

    // Load window icon
    sf::Image icon;
    if (!icon.loadFromFile("Icon/demo.png")) {
        cerr << "Error: Failed to load window icon." << endl;
        return -1;
    }

    // Create game window
    sf::RenderWindow window;
    CreateGameWindow(window, projectFullName, false, 120, 8, icon);

    // Set up textures
    Texture playerTex, gunTex, bulletTex, boundTex;
    playerTex.CreateTexture("checkerboardPlayer.png", true, false);
    gunTex.CreateTexture("guntex.png", true, false);
    bulletTex.CreateTexture("bullet.png", true, false);
    boundTex.CreateTexture("brickLine.png", true);

    // Set up player
    float moveRate = 4.0f;
    Player player(playerTex.getTex(), gunTex.getTex(), moveRate, window);
    player.SetPlayerPosition(iniPosition);

    // Set up boundaries
    sf::Sprite boundarySprites[4];
    CreateBoundaries(boundarySprites, boundTex.getTex(), window);

    // Set up enemy
    int noEnemies = 0;
    Enemy enemy;

    // Set up clocks
    sf::Clock clock, networkClock, serverResponseClock, gameClock;
    sf::Time posTimeInterval = sf::milliseconds(50);

    // Set up text elements
    helperText.setPosition(sf::Vector2f());
    helperText.setFont(robotoFont);
    helperText.setCharacterSize(22);
    helperText.setFillColor(sf::Color::Black);
    helperText.setOutlineColor(sf::Color::White);
    helperText.setOutlineThickness(2);
    helperText.setStyle(sf::Text::Bold);
    helperText.setString("Q - Quit\nWASD - Move\nLMB - Shoot\nMouseX/Y - Look");

    clockText.setPosition(sf::Vector2f(25, window.getSize().y - 50));
    clockText.setFont(dotoFont);
    clockText.setCharacterSize(20);
    clockText.setFillColor(sf::Color::Black);
    clockText.setOutlineColor(sf::Color::White);
    clockText.setOutlineThickness(1);
    clockText.setStyle(sf::Text::Italic | sf::Text::Bold);

    playerHealth.setPosition(window.getSize().x - 230, 10);
    playerHealth.setFont(robotoFont);
    playerHealth.setCharacterSize(24);
    playerHealth.setFillColor(sf::Color::Blue);
    playerHealth.setOutlineColor(sf::Color::White);
    playerHealth.setOutlineThickness(2);
    playerHealth.setStyle(sf::Text::Bold);

    enemyHealth.setPosition(window.getSize().x - 230, window.getSize().y - 30);
    enemyHealth.setFont(robotoFont);
    enemyHealth.setCharacterSize(24);
    enemyHealth.setFillColor(sf::Color::Red);
    enemyHealth.setOutlineColor(sf::Color::White);
    enemyHealth.setOutlineThickness(2);
    enemyHealth.setStyle(sf::Text::Bold);

    // Networking thread
    thread networkThread(NetworkingThread);

    // Player actions
    struct PlayerActions {
        unsigned sequenceNo;
        sf::Vector2f moveDir;
    };
    unsigned curSequenceNo = 1;
    vector<PlayerActions> actions;

    bool enemyFire = false;
    bool playerIdSet = false;

    // GAME LOOP
    while (window.isOpen()) {

        // Handle window events
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed: // Window close event
                window.close();
                break;

            case sf::Event::KeyReleased: // Handle key releases
                switch (event.key.scancode) {
                case sf::Keyboard::Scan::W:
                    player.MoveUp(false);
                    break;
                case sf::Keyboard::Scan::A:
                    player.MoveLeft(false);
                    break;
                case sf::Keyboard::Scan::S:
                    player.MoveDown(false);
                    break;
                case sf::Keyboard::Scan::D:
                    player.MoveRight(false);
                    break;
                }
                break;

            case sf::Event::KeyPressed: // Handle key presses
                switch (event.key.scancode) {
                case sf::Keyboard::Scan::W:
                    player.MoveUp(true);
                    break;
                case sf::Keyboard::Scan::A:
                    player.MoveLeft(true);
                    break;
                case sf::Keyboard::Scan::S:
                    player.MoveDown(true);
                    break;
                case sf::Keyboard::Scan::D:
                    player.MoveRight(true);
                    break;
                case sf::Keyboard::Scan::Q:
                    window.close();
                    Quit();
                    break;
                }
                break;

            case sf::Event::MouseButtonPressed: // Handle mouse clicks
                if (event.mouseButton.button == sf::Mouse::Left) {
                    player.Fire(bulletTex.getTex()); // Firing
                    // Add firing packet to the networking queue
                    sf::Packet playerFire;
                    playerFire << 1 << "PLAYER_FIRE" << player.GetPlayerSprite().getPosition()
                        << player.GetPlayerSprite().getRotation() << player.m_id
                        << player.m_bulletSpeed << 5.f;
                    {
                        lock_guard<mutex> lock(networkMutex);
                        toSend.push_back(playerFire);
                    }
                }
                break;
            }
        }

        // Process player movement
        sf::Vector2f playerMovDir = player.CheckMove();
        if (playerMovDir.x != 0 || playerMovDir.y != 0) {
            sf::Packet playerAct;
            playerAct << 1 << "PLAYER_ACTION_MOVE" << curSequenceNo << playerMovDir
                << player.GetMoveRate() << player.GetPlayerSprite().getPosition()
                << sf::Vector2f(player.GetMinX(), player.GetMinY())
                << sf::Vector2f(player.GetMaxX(), player.GetMaxY())
                << player.GetPlayerSprite().getRotation();
            {
                lock_guard<mutex> lock(networkMutex);
                toSend.push_back(playerAct);
            }
            actions.push_back({ curSequenceNo++, playerMovDir });
            player.MovePredicted(playerMovDir); // Optimistic movement
        }

        // Clear the window
        window.clear(sf::Color::Black);

        // Calculate delta time
        float deltaTime = clock.restart().asSeconds();

        // Process incoming packets from the server
        {
            lock_guard<mutex> lock(networkMutex);
            if (serverPackets.size() >= MAX_QUEUE_SIZE) {
                serverPackets.pop(); // Drop excess packets
            }
            while (!serverPackets.empty()) {
                sf::Packet curPacket = serverPackets.front();
                serverPackets.pop();

                string type;
                curPacket >> type;

                if (type == "PLAYER_POS") {
                    // Handle player position update
                    int serverSequenceNo;
                    curPacket >> serverSequenceNo;
                    sf::Vector2f newPos;
                    curPacket >> newPos;

                    if (player.GetPlayerSprite().getPosition() != newPos) {
                        player.GetPlayerSprite().setPosition(newPos);
                        player.GetGunSprite().setPosition(newPos);

                        for (const auto& action : actions) {
                            if (action.sequenceNo > serverSequenceNo) {
                                player.MovePredicted(action.moveDir);
                            }
                        }
                        actions.erase(
                            std::remove_if(actions.begin(), actions.end(),
                                [serverSequenceNo](const PlayerActions& action) {
                                    return action.sequenceNo == serverSequenceNo;
                                }),
                            actions.end());
                    }
                }

                if (type == "ENEMY_POS_ROT") {
                    // Handle enemy position and rotation update
                    curPacket >> enemyMoveDir >> enemyPos >> enemyRot >> serverTime;
                    if (lastEnemyPos != enemyPos) {
                        lastEnemyPos = enemyPos;
                    }
                    else {
                        enemyMoveDir = sf::Vector2f();
                    }
                    if (lastEnemyRot != enemyRot) {
                        lastEnemyRot = enemyRot;
                    }
                }

                if (type == "BULLET_HIT") {
                    // Handle bullet hit
                    string bulletId, objId;
                    curPacket >> bulletId >> objId;
                    if (objId == player.m_id) {
                        player.TakeDamage(BULLET_DAMAGE);
                    }
                    else {
                        enemy.TakeDamage(BULLET_DAMAGE);
                    }
                }

                if (type == "ENEMY_FIRE") {
                    // Handle enemy firing
                    curPacket >> enemyRot;
                    if (lastEnemyRot != enemyRot) {
                        lastEnemyRot = enemyRot;
                    }
                    enemyFire = true;
                }

                if (type == "ENEMIES_NO") {
                    // Reset game clock
                    gameClock.restart();

                    // Initialize enemy on connection
                    int no;
                    curPacket >> no;
                    noEnemies = no;
                    enemy.SetAll(playerTex.getTex(), gunTex.getTex(), moveRate, window);
                    if (!playerIdSet) {
                        player.m_id.append(to_string(no + 1));
                        playerIdSet = true;
                    }

                    // Enemy initial position calculations
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
                    // Handle enemy disconnection
                    int no;
                    curPacket >> no;
                    noEnemies = no;
                }
            }
        }

        // Draw game elements
        player.draw(window, deltaTime); // Draw player and bullets
        if (noEnemies > 0) { // If enemies are present
            if (enemyFire) {
                enemy.Fire(bulletTex.getTex(), lastEnemyRot);
                enemyFire = false;
            }
            predictedPos += enemy.MovePredicted(enemyMoveDir);
            predictedPos += (lastEnemyPos - predictedPos) * (deltaTime * 4);
            enemy.draw(window, deltaTime, predictedPos, lastEnemyRot);
        }
        else if (noEnemies == -1) { // If enemy forfeits
            window.close();
            WonForfeit();
        }

        DisplayBoundaries(boundarySprites, window); // Draw boundaries

        // Display UI elements
        clockText.setString(to_string(static_cast<int>(gameClock.getElapsedTime().asSeconds())));
        playerHealth.setString("Player Health: " + to_string(static_cast<int>(player.GetHealth())));
        if (noEnemies > 0) {
            enemyHealth.setString("Enemy Health: " + to_string(static_cast<int>(enemy.GetHealth())));
        }
        window.draw(clockText);
        window.draw(helperText);
        window.draw(playerHealth);
        window.draw(enemyHealth);

        // Check game-over conditions
        if (player.GetHealth() <= 0) {
            window.close();
            Lost();
        }
        if (enemy.GetHealth() <= 0) {
            window.close();
            WonKill();
        }

        // Checking for server disconnect
        if (appClose) {
            cerr << "closing game because server disconnected\n";
            window.close();
        }

        if (liveLost > totLiveLost) {
            cerr << "Error: Heavy SERVER_LIVE_CHECK failure, closing the application\n";
            window.close();
        }

        // Render everything
        window.display();
    }

    // Stop the networking thread and clean up resources
    isRunning = false;

    // Ensure the networking thread completes execution
    if (networkThread.joinable()) {
        networkThread.join();
    }

    // Clear the player action queue
    actions.clear();

    // Safely clear the list of outgoing packets
    {
        lock_guard<mutex> lock(networkMutex);
        toSend.clear();
    }

    return 0;

}