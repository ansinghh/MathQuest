#ifndef FIRSTSCREEN_H
#define FIRSTSCREEN_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

class FirstScreen
{
public:
    FirstScreen(sf::RenderWindow& window, int& activeScreen);

    void handleEvent(const sf::Event& event);
    void update();
    void render();

private:
    sf::RenderWindow& window;
    int& activeScreen;

    sf::Font font;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    sf::RectangleShape titleRectangle;
    sf::Text titleText;

    sf::RectangleShape nameLabelRectangle;
    sf::Text nameLabel;

    sf::RectangleShape nameInputRectangle;
    sf::Text nameInputText;

    sf::RectangleShape button;
    sf::Text buttonText;

    sf::SoundBuffer clickSoundBuffer;
    sf::SoundBuffer typingSoundBuffer;
    sf::Sound clickSound;
    sf::Sound typingSound;
};

#endif // FIRSTSCREEN_H
