#ifndef LEVEL1SCREEN_HPP
#define LEVEL1SCREEN_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <set>
#include <random>
#include <ctime>
using namespace sf;

class Level1Screen
{
public:
    Level1Screen(sf::RenderWindow& window, int& activeScreen);

    void handleEvent(const sf::Event& event);
    void update();
    void render();

private:
    sf::RenderWindow& window;
    int& activeScreen;
    sf::Font font;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    sf::Text titleText;
    int score;
    sf::CircleShape scoreShape;
    sf::Text scoreText;
    sf::SoundBuffer clickSoundBuffer;
    sf::Sound clickSound;
    sf::SoundBuffer correctAnswerBuffer;
    sf::Sound correctAnswerSound;
    sf::SoundBuffer wrongAnswerBuffer;
    sf::Sound wrongAnswerSound;

    std::vector<std::pair<std::string, std::string>> questionsAndAnswers;
    int currentQuestionIndex;

    int questionsAnswered;
    sf::Text questionText;
    sf::RectangleShape answerBox;
    sf::Text answerText;
    std::string answerInput;

    sf::RectangleShape enterButton;
    sf::Text enterButtonText;

    sf::RectangleShape tryAgainButton;
    sf::Text tryAgainButtonText;
    sf::RectangleShape continueButton;
    sf::Text continueButtonText;
    sf::RectangleShape continueToPongButton;
    sf::Text continueToPongButtonText;
    bool tryAgainButtonVisible;
    bool gameOver;

    void setupTitleText();
    void setupScoreText();
    void updateScoreTextPosition();
    void setupQuestionsAndAnswers();
    int getRandomQuestionIndex();
    void setupQuestionText();
    void setupAnswerBox();
    void setupAnswerText();
    void setupEnterButton();
    void setupTryAgainButton();
    void setupContinueButton();
    void setupContinueToPongButton();
    void handleQuestionEvent(const sf::Event& event);
    void handleEnterButtonEvent(const sf::Event& event);
    void handleTryAgainButtonEvent(const sf::Event& event);
    void handleContinueToPongButtonEvent(const sf::Event& event);
    void displayNextQuestion();
    void endGame();
    void resetGame();
    std::string formatScore(int score);
};

#endif // LEVEL1SCREEN_HPP
