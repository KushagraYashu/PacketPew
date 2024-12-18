//author: Kushagra

//cpp headers
#include <iostream>

//sfml headers
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

//custom headers
#include "Player.h"

using namespace std;

int main()
{
    //setting project name and version
    string versionNo = "1.0";
    string projectFullName = "PacketPew Client ";
    projectFullName.append(versionNo);

    //setting up antialiasing (smoothing the edges)
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    //creating a window
    sf::RenderWindow window(sf::VideoMode(1280, 720), projectFullName, sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize, settings);

    //setting up v-sync
    window.setVerticalSyncEnabled(true);

    //limiting the fps
    //window.setFramerateLimit(165);

    //setting up the icon
    sf::Image icon;
    if (!icon.loadFromFile("Icon/demo.png")) {
        std::cerr << "Error loading icon file" << std::endl;
        return -1;
    }
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    //setting up a texture
    sf::Texture checkerTex, bunnyTex, brickTex, playerTex;
    if (!checkerTex.loadFromFile("Textures/checkerboard.png")) {
        std::cerr << "error loading brick\n";
        return -1;
    }
    if (!bunnyTex.loadFromFile("Textures/bunny.png")) {
        std::cerr << "error loading brick\n";
        return -1;
    }
    if (!brickTex.loadFromFile("Textures/brickLine.png")) {
        std::cerr << "error loading brick\n";
        return -1;
    }
    if (!playerTex.loadFromFile("Textures/checkerboardPlayer.png")) {
        std::cerr << "error loading brick\n";
        return -1;
    }

    //properties of texture
    //smooth
    /*smoothing the texture
        Smoothing a texture makes pixel boundaries less visible (but the image a little more blurry), which can be desirable if it is up-scaled.
        smoothing can have unwanted effects since it also samples some pixels outside textured area, so if the sprite is at non integer location, it can mess up*/
    checkerTex.setSmooth(true);
    bunnyTex.setSmooth(true);
    brickTex.setSmooth(false);
    brickTex.setSmooth(true);

    //repeat
    /*texture will repeat itself if the sprite is bigger than the texture size
    */
    checkerTex.setRepeated(true);
    bunnyTex.setRepeated(true);
    brickTex.setRepeated(true);

    //creating a sprite
    sf::Sprite checkSprite, bunnySprite, brickSpriteWidth[2], brickSpriteHeight[2]; //by default it will be as big as the texture and at position (0,0)
    checkSprite.setTexture(checkerTex);
    bunnySprite.setTexture(bunnyTex);
    brickSpriteWidth[0].setTexture(brickTex);
    brickSpriteWidth[1].setTexture(brickTex);
    brickSpriteHeight[0].setTexture(brickTex);
    brickSpriteHeight[1].setTexture(brickTex);
    //resizing
    checkSprite.setScale(sf::Vector2f(0.2f, 0.2f));
    //setting texture rectangle
    bunnySprite.setTextureRect(sf::IntRect(10, 10, 350, 200)); //sf::IntRect defines a rectangle, it takes top left corner values and the rectangle size (width and height)
    brickSpriteWidth[0].setTextureRect(sf::IntRect(0, 0, window.getSize().x, 29));
    brickSpriteWidth[1].setTextureRect(sf::IntRect(0, 0, window.getSize().x, 29));
    brickSpriteHeight[0].setTextureRect(sf::IntRect(0, 0, window.getSize().y, 29));
    brickSpriteHeight[1].setTextureRect(sf::IntRect(0, 0, window.getSize().y, 29));
    //setting position
    sf::FloatRect checkSpriteGlobalBounds = checkSprite.getGlobalBounds(); //global bounds returns the transform rect in global (window) space, top left corner (left, top) is the actual coordinate in the window, width height are the width height
    bunnySprite.setPosition(sf::Vector2f(checkSpriteGlobalBounds.left, checkSpriteGlobalBounds.top + checkSpriteGlobalBounds.height));
    brickSpriteWidth[0].setPosition(sf::Vector2f(0.f, -5.f));
    brickSpriteWidth[1].setOrigin(0.f, brickSpriteHeight[1].getGlobalBounds().top + brickSpriteHeight[1].getGlobalBounds().height);
    brickSpriteWidth[1].setPosition(sf::Vector2f(0.f, window.getSize().y + 5.f));
    brickSpriteHeight[0].setPosition(brickSpriteHeight[0].getGlobalBounds().top + brickSpriteHeight[0].getGlobalBounds().height, 0.f);
    brickSpriteHeight[1].setPosition(window.getSize().x, 0.f);
    //set color
    checkSprite.setColor(sf::Color::Magenta);
    //set rotation
    bunnySprite.setRotation(15.f); //rotates clockwise, about the top-left corner, completely overwrites the previous rotation
    bunnySprite.rotate(10.f); //this rotates based on the previous rotation, clockwise as well
    bunnySprite.setRotation(0.f); //overwriting everything to 0 degrees of rotation
    //setting origin for the transformations, by default everything is about the top-left corner, lets set it to the centre of the sprite
    bunnySprite.setOrigin(sf::Vector2f(bunnySprite.getLocalBounds().width / 2, bunnySprite.getLocalBounds().height / 2));
    bunnySprite.rotate(15.f); //rotating about the new origin (centre)
    bunnySprite.setPosition((float)window.getSize().x / 2, (float)window.getSize().y / 2);
    brickSpriteHeight[0].rotate(90);
    brickSpriteHeight[1].rotate(90);

    //setting up the fps text with a background shape
    sf::Text fpsText;
    sf::RectangleShape fpsBG;
    sf::Font dotoFont, robotoFont;
    if (!dotoFont.loadFromFile("Fonts/Doto/Doto-Bold.ttf")) {
        std::cerr << "Error loading font file" << std::endl;
        return -1;
    }
    if (!robotoFont.loadFromFile("Fonts/Roboto/Roboto-Regular.ttf")) {
        std::cerr << "Error loading font file" << std::endl;
        return -1;
    }
    fpsBG.setPosition(sf::Vector2f(3.f, 3.f));
    fpsBG.setSize(sf::Vector2f(75.f, 20.f));
    fpsBG.setFillColor(sf::Color::Yellow);
    fpsBG.setOutlineThickness(2.f);
    fpsBG.setOutlineColor(sf::Color::Black);
    fpsText.setFont(robotoFont);
    fpsText.setCharacterSize(18);
    fpsText.setFillColor(sf::Color::Black);
    fpsText.setStyle(sf::Text::Bold);
    float fps = 0.0f;

    //setting up a clock (for now, only being used by FPS)
    sf::Clock clock;

    //trying custom Player Class
    Player player(playerTex, 1.0f, window);

    //move bools
    bool moveUp = false, moveDown = false, moveRight = false, moveLeft = false;

    //GAME LOOP
    //run the program as long as the window is open
    while (window.isOpen()) {

        //checking all the window events that were triggered since the last iteration of this game loop
        sf::Event event;
        while (window.pollEvent(event)) {

            //checking for different types of events
            switch (event.type) {

            //if the event is close, close the window
            case sf::Event::Closed: 
                cout << endl << "closing the window";
                window.close();
                break;

            case sf::Event::KeyReleased:
                if (event.key.scancode == sf::Keyboard::Scan::W) {
                    moveUp = false;
                }
                if (event.key.scancode == sf::Keyboard::Scan::A) {
                    moveLeft = false;
                }
                if (event.key.scancode == sf::Keyboard::Scan::S) {
                    moveDown = false;
                }
                if (event.key.scancode == sf::Keyboard::Scan::D) {
                    moveRight = false;
                }
                break;

            case sf::Event::KeyPressed:
                if (event.key.scancode == sf::Keyboard::Scan::W) {
                    moveUp = true;
                }
                if (event.key.scancode == sf::Keyboard::Scan::A) {
                    moveLeft = true;
                }
                if (event.key.scancode == sf::Keyboard::Scan::S) {
                    moveDown = true;
                }
                if (event.key.scancode == sf::Keyboard::Scan::D) {
                    moveRight = true;
                }
                break;
            
            //text-entered event
            /*case sf::Event::TextEntered:
                if (event.text.unicode == 13) {
                    cout << endl;
                }
                else {
                    cout << static_cast<char>(event.text.unicode) << endl;
                }
                break;*/

            //keypressed event
            //case sf::Event::KeyPressed: //if you want to do something when the key is hold down, use keyreleased since the keypressed will have a repeat delay and will not fire the event at all times
            //    //how you might ask? set a bool when a particular key is pressed, do that task when the bool is true, make the bool false when you recieve a keyreleased for that key, simple

            //    //comparing the scancode
            //    if (event.key.scancode == sf::Keyboard::Scan::Escape) { //using scancode is advised as it maps to the exact key location irrespective of the keyboard layout
            //        cout << "pressed Escape" << endl;
            //    }

            //    //printing the scancode (it is the physical code of the key)
            //    cout << event.key.scancode << endl;

            //    //get description of the key pressed
            //    cout << "key pressed: " << sf::Keyboard::getDescription(event.key.scancode).toAnsiString() << endl;
            //    break;

            //the same sort of operations can be done with KeyReleased

            //mouse-wheel-scrolled event
            //case sf::Event::MouseWheelScrolled:
            //    //comparing if the vertical wheel is moved, SFML also supports horizontal wheel if that exists
            //    if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
            //        cout << "vertical wheel moved" << endl;
            //    }

            //    //wheel movement value
            //    cout << event.mouseWheelScroll.delta << endl;

            //    //it can also tell the current position of the mouse
            //    cout << event.mouseWheelScroll.x << "\t" << event.mouseWheelScroll.y << endl;
            //    break;

            //mouse-button-pressed event
            //case sf::Event::MouseButtonPressed:
            //    //comparing which button was pressed
            //    if (event.mouseButton.button == sf::Mouse::Right) {
            //        cout << "right mouse button pressed\n";
            //    }

            //    //it can also tell the current position of the mouse
            //    cout << event.mouseButton.x << "\t" << event.mouseButton.y << "\n";
            //    break;

            //same can be done with the mouse-button-released event

            //mouse-moved event
            //case sf::Event::MouseMoved:
            //    //it can only track the mouse position in the window area and not on the whole display, also it fires event when the window is not in focus
            //    //it can display the position the mouse has moved to (new position)
            //    cout << event.mouseMove.x << "\t" << event.mouseMove.y << "\n";
            //    break;

            //mouse-entered event
            case sf::Event::MouseEntered:
                //triggered when the mouse enters the window
                cout << "mouse entered the window\n";
                break;

            //same for mouse-left
            case sf::Event::MouseLeft:
                cout << "mouse left the window\n";
                break;

            //SFML has events for joysticks as well, but i wont be using those hopefully

            //if the event is resize, show that new resolution
            case sf::Event::Resized: 
                cout << endl << "window has been resized to " << event.size.width << " X " << event.size.height << endl;
                break;

            //if none of the above
            default: //do nothing
                break;
            }
        }

        //real-time checking of global input devices (keyboard, mouse, joysticks)
        //real-time checking increases CPU usage
        //these can return true even if the window is not in focus
        //consider real-time checking as, "is this button currently pressed?" whereas event is "this key was pressed"
        /*if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Left)) { //you can use just ::Left and omit the Scan::Left but scan is more accurate since it will check the actual physical left arrow key no matter the keyboard layout
            cout << "Left arrow key is pressed\n";
        }*/

        //mouse real time checking
        /*if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            cout << "Left mouse button pressed\n";
        }*/
        //global position (relative to the desktop)
        //cout << sf::Mouse::getPosition().x <<"\t" << sf::Mouse::getPosition().y << endl;

        //local position (reative to the window)
        //it does return a value even if the mouse is not in the window, it will just adjust the value based on the window coordinates
        //cout << sf::Mouse::getPosition(window).x <<"\t" << sf::Mouse::getPosition(window).y << endl;

        //set the global position of the mouse
        //sf::Mouse::setPosition(sf::Vector2i(0.f, 0.f));

        //set the local position of the mouse
        //sf::Mouse::setPosition(sf::Vector2i(0.f, 0.f), window);

        //calculating and setting fps
        float frameTime = clock.restart().asSeconds();
        fps = 1.0f / frameTime;
        fpsText.setString("FPS: " + to_string(static_cast<int>(fps)));

        //moving logic (based on move bools)
        if (moveUp) {
            player.Move(0.f, -1.f);
        }
        if (moveDown) {
            player.Move(0.f, 1.f);
        }
        if (moveRight) {
            player.Move(1.f, 0.f);
        }
        if (moveLeft) {
            player.Move(-1.f, 0.f);
        }

        //clearing the window, it is mandatory otherwise the contents from last frame will be present in the current frame
        window.clear(sf::Color(0, 0, 0, 255));

        //drawing sprite
        //window.draw(checkSprite);
        //window.draw(bunnySprite);
        window.draw(player.getPlayerSprite());
        window.draw(brickSpriteWidth[0]);
        window.draw(brickSpriteWidth[1]);
        window.draw(brickSpriteHeight[0]);
        window.draw(brickSpriteHeight[1]);
        //drawing the fps text, keep this at the end so that it can be drawn over other elements
        window.draw(fpsBG);
        //doing this calculation in game loop because the fpsText will not have anytext before the loop and hence 0 width and 0 height which leads to 0, 0 origin
        fpsText.setOrigin(fpsText.getLocalBounds().width / 2, fpsText.getLocalBounds().height / 2);
        fpsText.setPosition(fpsBG.getGlobalBounds().left + (fpsBG.getGlobalBounds().width / 2), fpsBG.getGlobalBounds().top + (fpsBG.getGlobalBounds().height / 2 - 3.f));
        window.draw(fpsText);

        //displaying everything that has been rendered to the window so far, renders every draw command
        window.display();
    }

    return 0;
}
