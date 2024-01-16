#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <string>
#include <random>
#include <chrono>
#include <thread>
using namespace sf;

#define SIZE 4

struct Pair {
    std::string question;
    std::string answer;
};

struct Card {
    sf::RectangleShape rect;
    sf::RectangleShape flipButton;
    sf::Text buttonText;
    Pair pair;
    std::string text;
    bool revealed;
    bool matched;
    bool isQuestion;
};

class MemoryGame {
private:
    sf::RenderWindow window;
    std::vector<Card> cards;
    sf::Font font;
    int lastCard;
    bool wait;
    sf::Clock clock;
    sf::Texture backgroundTexture;  // Background texture
    sf::Sprite backgroundSprite;    // Background sprite

    // Instructions screen
    bool showInstructions;

public:
    MemoryGame();

    Card createCard(Pair pair, bool isQuestion);

    int countdown_timer(sf::RenderWindow& window, sf::Text countdown, int total_start);

    void Run();
};
