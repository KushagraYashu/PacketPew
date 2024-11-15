//author: Kushagra

//cpp headers
#include <iostream>

//sfml headers
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

using namespace std;

int main()
{
    //setting project name and version
    string versionNo = "1.0";
    string projectFullName = "PacketPew Server ";
    projectFullName.append(versionNo);

    //creating a window
    sf::RenderWindow window(sf::VideoMode(1280, 720), projectFullName, sf::Style::Titlebar | sf::Style::Close);

    //setting up v-sync
    //window.setVerticalSyncEnabled(true);

    //limiting the fps
    //window.setFramerateLimit(165);

    //setting up the icon
    sf::Image icon;
    if (!icon.loadFromFile("Icon/demo.png")) {
        std::cerr << "Error loading icon file" << std::endl;
        return -1;
    }
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    //setting up the fps text
    sf::Text fpsText;
    sf::Font dotoFont, robotoFont;
    if (!dotoFont.loadFromFile("Fonts/Doto/Doto-Bold.ttf")) {
        std::cerr << "Error loading font file" << std::endl;
        return -1;
    }
    if (!robotoFont.loadFromFile("Fonts/Roboto/Roboto-Regular.ttf")) {
        std::cerr << "Error loading font file" << std::endl;
        return -1;
    }
    fpsText.setFont(robotoFont);
    fpsText.setCharacterSize(18);
    fpsText.setFillColor(sf::Color::Cyan);
    fpsText.setPosition(5.f, 5.f);
    float fps = 0.0f;

    //setting up a clock (for now, only being used by FPS)
    sf::Clock clock;

    //run the program as long as the window is open
    while (window.isOpen()) {

        //checking all the window events that were triggered since the last iteration of this game loop
        sf::Event event;
        while (window.pollEvent(event)) {

            //if the event is close, close the window
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        //calculating and setting fps
        float frameTime = clock.restart().asSeconds();
        fps = 1.0f / frameTime;
        fpsText.setString("FPS: " + to_string(static_cast<int>(fps)));

        //clearing the window
        window.clear(sf::Color(0, 0, 0, 255));

        //drawing the fps text
        window.draw(fpsText);

        //displaying everything that has been rendered to the window so far
        window.display();
    }

    return 0;
}
