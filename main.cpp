#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <set>
#include <random>
#include <ctime>
#include "FirstScreen.h"
#include "Level1Screen.hpp"
#include "Memorygame.hpp"
#include "pong.hpp" // Include the PongGame header
using namespace sf;
int main()
{
    sf::RenderWindow window(sf::VideoMode(1500, 1000), "MathQuest");

    int activeScreen = 1; // First screen is active by default

    FirstScreen firstScreen(window, activeScreen);
    Level1Screen level1Screen(window, activeScreen);

    // Remove the MemoryGame instance from the main function

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            if (activeScreen == 1)
            {
                firstScreen.handleEvent(event);
            }
            else if (activeScreen == 2)
            {
                level1Screen.handleEvent(event);
            }
        }

        if (activeScreen == 1)
        {
            firstScreen.update();
        }
        else if (activeScreen == 2)
        {
            level1Screen.update();
        }
        else if (activeScreen == 3)
        {
            
            std::srand(static_cast<unsigned int>(std::time(nullptr)));
            MemoryGame game;
            game.Run();
            activeScreen = 2; // Set activeScreen to 2 after MemoryGame finishes
        }
        else if (activeScreen == 4) // Pong game
        {
            PongGame pongGame(window);
            pongGame.run();
            activeScreen = 2; // Set activeScreen to 2 after PongGame finishes
        }

        window.clear();

        if (activeScreen == 1)
        {
            firstScreen.render();
        }
        else if (activeScreen == 2)
        {
            level1Screen.render();
        }

        window.display();
    }

    return 0;
}
