#include "pong.hpp"
using namespace sf;
#include <ctime>
#include <cstdlib>
#include <iostream>

const float PongGame::PADDLE_WIDTH = 20.0f;
const float PongGame::PADDLE_HEIGHT = 100.0f;
const float PongGame::PADDLE_SPEED = 300.0f;
const float PongGame::BALL_RADIUS = 10.0f;
const float PongGame::BALL_VELOCITY_X = 500.0f;
const float PongGame::BALL_VELOCITY_Y = 400.0f;
const unsigned int PongGame::WINDOW_WIDTH = 1500; // or whatever you want
const unsigned int PongGame::WINDOW_HEIGHT = 1000; // or whatever you want

PongGame::PongGame(sf::RenderWindow& win) : window(win) // Change this line
{
    srand(time(nullptr));

    if (!font.loadFromFile("myfnt.ttf"))
    {
        std::cerr << "Failed to load font" << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

void PongGame::run()
{
    sf::CircleShape scoreCircle1(50);
    scoreCircle1.setFillColor(sf::Color::Transparent);
    scoreCircle1.setOutlineColor(sf::Color::White);
    scoreCircle1.setOutlineThickness(5);
    scoreCircle1.setPosition(150, 30);

    sf::CircleShape scoreCircle2 = scoreCircle1;
    scoreCircle2.setPosition(WINDOW_WIDTH - 200, 30);

    sf::Text scoreText1;
    scoreText1.setFont(font);
    scoreText1.setCharacterSize(30);
    scoreText1.setFillColor(sf::Color::White);

    sf::Text scoreText2 = scoreText1;

    sf::RectangleShape paddle1(sf::Vector2f(PADDLE_WIDTH, PADDLE_HEIGHT));
    paddle1.setPosition(50.0f, WINDOW_HEIGHT / 2.0f - PADDLE_HEIGHT / 2.0f);
    paddle1.setFillColor(sf::Color::Blue);

    sf::RectangleShape paddle2(sf::Vector2f(PADDLE_WIDTH, PADDLE_HEIGHT));
    paddle2.setPosition(WINDOW_WIDTH - 50.0f - PADDLE_WIDTH, WINDOW_HEIGHT / 2.0f - PADDLE_HEIGHT / 2.0f);
    paddle2.setFillColor(sf::Color::Red);

    sf::CircleShape ball(BALL_RADIUS);
    ball.setPosition(WINDOW_WIDTH / 2.0f - BALL_RADIUS, WINDOW_HEIGHT / 2.0f - BALL_RADIUS);
    ball.setFillColor(sf::Color::White);

    sf::RectangleShape answerBox;
    setupAnswerBox(answerBox);

    sf::Text answerText;
    setupAnswerText(answerBox, answerText);

    sf::Text questionText;
    questionText.setFont(font);
    questionText.setCharacterSize(30);
    questionText.setFillColor(sf::Color::White);
    questionText.setPosition(window.getSize().x / 2.0f - 150.0f, answerBox.getPosition().y - 70.0f);

    sf::RectangleShape enterButton(sf::Vector2f(150.0f, 50.0f));
    enterButton.setPosition(WINDOW_WIDTH / 2.0f - enterButton.getSize().x / 2.0f, answerBox.getPosition().y + answerBox.getSize().y + 10.0f);
    enterButton.setFillColor(sf::Color::Green);

    sf::Text buttonText;
    buttonText.setFont(font);
    buttonText.setString("Enter");
    buttonText.setCharacterSize(30);
    buttonText.setFillColor(sf::Color::Black);
    buttonText.setPosition(enterButton.getPosition().x + enterButton.getSize().x / 2.0f - buttonText.getGlobalBounds().width / 2.0f,
                           enterButton.getPosition().y + enterButton.getSize().y / 2.0f - buttonText.getGlobalBounds().height / 1.5f);

    int player1Score = 0;
    int player2Score = 0;
    bool gamePaused = false;
    std::string playerAnswer;
    int answer;
    sf::Vector2f ballVelocity(BALL_VELOCITY_X, BALL_VELOCITY_Y);

    sf::Clock clock;

    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (gamePaused && event.type == sf::Event::TextEntered)
            {
                if (event.text.unicode == '\b' && playerAnswer.size() > 0)
                    playerAnswer.pop_back();
                else if (event.text.unicode >= '0' && event.text.unicode <= '9')
                    playerAnswer += static_cast<char>(event.text.unicode);

                answerText.setString(playerAnswer);
            }
            else if (gamePaused && event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left &&
                    enterButton.getGlobalBounds().contains(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y)))
                {
                    if (!playerAnswer.empty() && isPositiveInteger(playerAnswer) && std::stoi(playerAnswer) == answer)
                    {
                        if (ball.getPosition().x < 0)
                            player2Score++;
                        else
                            player1Score++;
                    }
                    else
                    {
                        if (!playerAnswer.empty())
                        {
                            if (ball.getPosition().x < 0)
                                player1Score--;
                            else
                                player2Score--;
                        }
                    }

                    gamePaused = false;
                    ballVelocity = sf::Vector2f(BALL_VELOCITY_X, BALL_VELOCITY_Y);
                    playerAnswer.clear();
                    ball.setPosition(WINDOW_WIDTH / 2.0f - BALL_RADIUS, WINDOW_HEIGHT / 2.0f - BALL_RADIUS);
                }
            }
        }

        if (!gamePaused)
        {
            sf::Time deltaTime = clock.restart();
            float dt = deltaTime.asSeconds();

            // Update paddle positions
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && paddle1.getPosition().y > 0)
                paddle1.move(0, -PADDLE_SPEED * dt);
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && paddle1.getPosition().y < WINDOW_HEIGHT - PADDLE_HEIGHT)
                paddle1.move(0, PADDLE_SPEED * dt);

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && paddle2.getPosition().y > 0)
                paddle2.move(0, -PADDLE_SPEED * dt);
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && paddle2.getPosition().y < WINDOW_HEIGHT - PADDLE_HEIGHT)
                paddle2.move(0, PADDLE_SPEED * dt);

            // Update ball position
            ball.move(ballVelocity * dt);

            // Ball-paddle collision
            if ((ball.getGlobalBounds().intersects(paddle1.getGlobalBounds()) && ballVelocity.x < 0) ||
                (ball.getGlobalBounds().intersects(paddle2.getGlobalBounds()) && ballVelocity.x > 0))
            {
                ballVelocity.x = -ballVelocity.x;
            }

            // Ball-wall collision
            if (ball.getPosition().y <= 0 || ball.getPosition().y >= WINDOW_HEIGHT - BALL_RADIUS * 2)
                ballVelocity.y = -ballVelocity.y;

            // Ball out of screen
            if (ball.getPosition().x < 0 || ball.getPosition().x > WINDOW_WIDTH)
            {
                std::pair<std::string, int> question = generateLinearEquation();
                questionText.setString(question.first);
                answer = question.second;
                gamePaused = true;
            }
        }

        scoreText1.setString(std::to_string(player1Score));
        scoreText1.setPosition(scoreCircle1.getPosition().x + scoreCircle1.getRadius() - scoreText1.getGlobalBounds().width / 2.0f,
                               scoreCircle1.getPosition().y + scoreCircle1.getRadius() - scoreText1.getGlobalBounds().height / 1.5f);

        scoreText2.setString(std::to_string(player2Score));
        scoreText2.setPosition(scoreCircle2.getPosition().x + scoreCircle2.getRadius() - scoreText2.getGlobalBounds().width / 2.0f,
                               scoreCircle2.getPosition().y + scoreCircle2.getRadius() - scoreText2.getGlobalBounds().height / 1.5f);

        // Draw everything
        window.clear();
        window.draw(paddle1);
        window.draw(paddle2);
        window.draw(ball);
        window.draw(scoreCircle1);
        window.draw(scoreCircle2);
        window.draw(scoreText1);
        window.draw(scoreText2);

        if (gamePaused)
        {
            window.draw(answerBox);
            window.draw(answerText);
            window.draw(questionText);
            window.draw(enterButton);
            window.draw(buttonText);
        }

        window.display();
    }
}

std::pair<std::string, int> PongGame::generateLinearEquation()
{
    int x = rand() % 10 + 1;
    int b = rand() % 10;
    int a = rand() % 10;
    int y = a * x + b;
    std::string equation = std::to_string(a) + "x + " + std::to_string(b) + " = " + std::to_string(y);
    return std::make_pair(equation, x);
}

bool PongGame::isPositiveInteger(const std::string& str)
{
    if (str.empty())
        return false;

    for (char c : str)
    {
        if (!std::isdigit(c))
            return false;
    }

    return true;
}

void PongGame::setupAnswerBox(sf::RectangleShape& answerBox)
{
    sf::Vector2f boxSize(400.0f, 60.0f);
    answerBox.setSize(boxSize);
    answerBox.setFillColor(sf::Color::White);
    answerBox.setPosition(window.getSize().x / 2.0f - boxSize.x / 2.0f, window.getSize().y / 2.0f);
}

void PongGame::setupAnswerText(sf::RectangleShape& answerBox, sf::Text& answerText)
{
    unsigned int fontSize = 30;
    sf::Color textColor = sf::Color::Black;
    answerText.setFont(font);
    answerText.setCharacterSize(fontSize);
    answerText.setFillColor(textColor);
    answerText.setPosition(answerBox.getPosition().x + 10.0f, answerBox.getPosition().y + 10.0f);
}

void PongGame::resetGame(sf::RectangleShape& paddle1, sf::RectangleShape& paddle2, sf::CircleShape& ball, int& player1Score, int& player2Score, bool& gamePaused, sf::Vector2f& ballVelocity)
{
    paddle1.setPosition(50.0f, WINDOW_HEIGHT / 2.0f - PADDLE_HEIGHT / 2.0f);
    paddle2.setPosition(WINDOW_WIDTH - 50.0f - PADDLE_WIDTH, WINDOW_HEIGHT / 2.0f - PADDLE_HEIGHT / 2.0f);
    ball.setPosition(WINDOW_WIDTH / 2.0f - BALL_RADIUS, WINDOW_HEIGHT / 2.0f - BALL_RADIUS);
    player1Score = 0;
    player2Score = 0;
    gamePaused = false;
    ballVelocity = sf::Vector2f(BALL_VELOCITY_X, BALL_VELOCITY_Y);
}
