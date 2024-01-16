#ifndef PONG_HPP
#define PONG_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include <utility>

class PongGame
{
public:
    static const float PADDLE_WIDTH;
    static const float PADDLE_HEIGHT;
    static const float PADDLE_SPEED;
    static const float BALL_RADIUS;
    static const float BALL_VELOCITY_X;
    static const float BALL_VELOCITY_Y;
    static const unsigned int WINDOW_WIDTH;
    static const unsigned int WINDOW_HEIGHT;

    PongGame(sf::RenderWindow& window); // Change this line
    void run();

private:
    sf::RenderWindow& window; // Change this line
    sf::Font font;
    std::pair<std::string, int> generateLinearEquation();
    bool isPositiveInteger(const std::string& str);
    void setupAnswerBox(sf::RectangleShape& answerBox);
    void setupAnswerText(sf::RectangleShape& answerBox, sf::Text& answerText);
    void resetGame(sf::RectangleShape& paddle1, sf::RectangleShape& paddle2, sf::CircleShape& ball, int& player1Score, int& player2Score, bool& gamePaused, sf::Vector2f& ballVelocity);
};

#endif
