#include <SFML/Graphics.hpp>
#include "Level1Screen.hpp"
using namespace sf;

Level1Screen::Level1Screen(sf::RenderWindow& window, int& activeScreen)
    : window(window), activeScreen(activeScreen), score(0), currentQuestionIndex(-1),
      questionsAnswered(0), tryAgainButtonVisible(false), gameOver(false)
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    if (!font.loadFromFile("font2.ttf")) {
        std::cout << "Error: Failed to load font\n";
    }
    if (!backgroundTexture.loadFromFile("background2.png")) {
        std::cout << "Error: Failed to load background\n";
    }
    backgroundSprite.setTexture(backgroundTexture);
    setupTitleText();
    setupScoreText();
    if (!clickSoundBuffer.loadFromFile("click.wav")) {
        std::cout << "Error: Failed to load click sound\n";
    }
    clickSound.setBuffer(clickSoundBuffer);

    if (!correctAnswerBuffer.loadFromFile("sound1.wav")) {
        std::cout << "Error: Failed to load correct answer sound\n";
    }
    correctAnswerSound.setBuffer(correctAnswerBuffer);

    if (!wrongAnswerBuffer.loadFromFile("sound2.wav")) {
        std::cout << "Error: Failed to load wrong answer sound\n";
    }
    wrongAnswerSound.setBuffer(wrongAnswerBuffer);
    setupQuestionsAndAnswers();
    setupQuestionText();
    setupAnswerBox();
    setupAnswerText();
    setupEnterButton();
    setupTryAgainButton();
    setupContinueButton();
    setupContinueToPongButton();
}

void Level1Screen::handleEvent(const sf::Event& event)
{
    if (event.type == sf::Event::TextEntered) {
        handleQuestionEvent(event);
    }
    else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        handleEnterButtonEvent(event);
        handleTryAgainButtonEvent(event);
        handleContinueToPongButtonEvent(event);
    }
}

void Level1Screen::update()
{
    // Update the score text position
    updateScoreTextPosition();
}

void Level1Screen::render()
{
    // Draw the background first
    window.draw(backgroundSprite);

    // Draw the title text
    window.draw(titleText);

    // Draw the question and answer input box
    window.draw(questionText);
    window.draw(answerBox);
    window.draw(answerText);

    // Draw the enter button if the game is not over
    if (!gameOver) {
        window.draw(enterButton);
        window.draw(enterButtonText);
    }

    // Draw the score text
    window.draw(scoreShape);
    window.draw(scoreText);

    // Draw the try again button if visible
    if (tryAgainButtonVisible) {
        window.draw(tryAgainButton);
        window.draw(tryAgainButtonText);
    }

    // Draw the continue button if visible
    if (gameOver) {
        window.draw(continueButton);
        window.draw(continueButtonText);
        window.draw(continueToPongButton);
        window.draw(continueToPongButtonText);
    }
}

void Level1Screen::setupTitleText()
{
    unsigned int fontSize = 60;
    sf::Color textColor = sf::Color::White;
    titleText.setFont(font);
    titleText.setCharacterSize(fontSize);
    titleText.setFillColor(textColor);
    titleText.setString("Level 1: Easy");
    sf::FloatRect textBounds = titleText.getLocalBounds();
    titleText.setOrigin(textBounds.width / 2.f, textBounds.height / 2.f);
    titleText.setPosition(window.getSize().x / 3.5f, window.getSize().y / 4.5f);
}

void Level1Screen::setupScoreText()
{
    unsigned int fontSize = 35;
    sf::Color textColor = sf::Color::Red;
    scoreText.setFont(font);
    scoreText.setCharacterSize(fontSize);
    scoreText.setFillColor(textColor);
    scoreText.setString("Your Score:\n      000");
    float circleRadius = 100.0f;
    scoreShape.setRadius(circleRadius);
    scoreShape.setFillColor(sf::Color::Yellow);
    scoreShape.setPosition(window.getSize().x - circleRadius * 2 - 40.0f, 40.0f);
    updateScoreTextPosition();
}

void Level1Screen::updateScoreTextPosition()
{
    // Position the score text within the circular shape
    sf::FloatRect scoreTextBounds = scoreText.getLocalBounds();
    float scoreTextX = scoreShape.getPosition().x + scoreShape.getRadius() - scoreTextBounds.width / 2.0f;
    float scoreTextY = scoreShape.getPosition().y + scoreShape.getRadius() - scoreTextBounds.height / 2.0f;
    scoreText.setPosition(scoreTextX, scoreTextY);
}

void Level1Screen::setupQuestionsAndAnswers()
{
    questionsAndAnswers.clear(); // Clear the existing questions and answers

    const int numQuestions = 10; // Number of questions to generate

    for (int i = 0; i < numQuestions; i++) {
        std::string question;
        std::string answer;

        if (i % 5 == 0 || i % 5 == 1) {
            // Additional questions
            if (i % 5 == 0) {
                question = "Find the missing number: 2, 5, 10, 17, ?, 37, 50";
                answer = "26";
            }
            else {
                question = "Find the next number in the sequence: 1, 4, 9, 16, 25, ?";
                answer = "36";
            }
        }
        else if (i % 5 == 2) {
            // Linear equation: ax + b = c
            int a, b, c, x;
            do {
                a = std::rand() % 10 + 1; // Random coefficient between 1 and 10
                b = std::rand() % 10 + 1; // Random coefficient between 1 and 10
                c = a * b;
                x = c / a;
            } while (x <= 0 || x * a != c);

            question = "Solve for x: " + std::to_string(a) + "x + " + std::to_string(b) + " = " + std::to_string(c);
            answer = std::to_string(x);
        }
        else {
            // Quadratic equation: ax^2 + bx + c = 0
            int a, b, c, x;
            do {
                a = std::rand() % 5 + 1; // Random coefficient between 1 and 5
                b = std::rand() % 10 - 5; // Random coefficient between -5 and 4
                c = std::rand() % 10 - 5; // Random coefficient between -5 and 4
                int discriminant = b * b - 4 * a * c;
                if (discriminant >= 0) {
                    int x1 = (-b + std::sqrt(discriminant)) / (2 * a);
                    int x2 = (-b - std::sqrt(discriminant)) / (2 * a);
                    if (x1 > 0 && x2 > 0 && x1 == x2) {
                        x = x1;
                        break;
                    }
                }
            } while (true);

            question = "Solve for x: " + std::to_string(a) + "x^2 + " + std::to_string(b) + "x + " + std::to_string(c) + " = 0";
            answer = std::to_string(x);
        }

        questionsAndAnswers.push_back(std::make_pair(question, answer));
    }
}

int Level1Screen::getRandomQuestionIndex()
{
    int randomIndex = std::rand() % questionsAndAnswers.size();
    return randomIndex;
}

void Level1Screen::setupQuestionText()
{
    unsigned int fontSize = 50;
    sf::Color textColor = sf::Color::White;
    questionText.setFont(font);
    questionText.setCharacterSize(fontSize);
    questionText.setFillColor(textColor);
    questionText.setString("");
    questionText.setPosition(window.getSize().x / 6.5f, window.getSize().y / 2.5f);
    displayNextQuestion();
}

void Level1Screen::setupAnswerBox()
{
    sf::Vector2f boxSize(400.0f, 60.0f);
    answerBox.setSize(boxSize);
    answerBox.setFillColor(sf::Color::White);
    answerBox.setPosition(window.getSize().x / 3.5f - boxSize.x / 2.0f, window.getSize().y / 2.0f);
}

void Level1Screen::setupAnswerText()
{
    unsigned int fontSize = 30;
    sf::Color textColor = sf::Color::Black;
    answerText.setFont(font);
    answerText.setCharacterSize(fontSize);
    answerText.setFillColor(textColor);
    answerText.setString("");
    answerText.setPosition(answerBox.getPosition().x + 10.0f, answerBox.getPosition().y + 10.0f);
}

void Level1Screen::setupEnterButton()
{
    sf::Vector2f buttonSize(200.0f, 60.0f);
    enterButton.setSize(buttonSize);
    enterButton.setFillColor(sf::Color::Green);
    enterButton.setPosition(answerBox.getPosition().x, answerBox.getPosition().y + answerBox.getSize().y + 20.0f);

    unsigned int fontSize = 30;
    sf::Color textColor = sf::Color::Black;
    enterButtonText.setFont(font);
    enterButtonText.setCharacterSize(fontSize);
    enterButtonText.setFillColor(textColor);
    enterButtonText.setString("Enter");
    sf::FloatRect textBounds = enterButtonText.getLocalBounds();
    enterButtonText.setOrigin(textBounds.width / 2.f, textBounds.height / 2.f);
    enterButtonText.setPosition(enterButton.getPosition().x + buttonSize.x / 2.0f, enterButton.getPosition().y + buttonSize.y / 2.0f - textBounds.height / 2.0f);
}

void Level1Screen::setupTryAgainButton()
{
    sf::Vector2f buttonSize(200.0f, 60.0f);
    tryAgainButton.setSize(buttonSize);
    tryAgainButton.setFillColor(sf::Color::Green);

    unsigned int fontSize = 30;
    sf::Color textColor = sf::Color::Black;
    tryAgainButtonText.setFont(font);
    tryAgainButtonText.setCharacterSize(fontSize);
    tryAgainButtonText.setFillColor(textColor);
    tryAgainButtonText.setString("Try Again");
}

void Level1Screen::setupContinueButton()
{
    sf::Vector2f buttonSize(200.0f, 60.0f);
    continueButton.setSize(buttonSize);
    continueButton.setFillColor(sf::Color::Green);

    unsigned int fontSize = 30;
    sf::Color textColor = sf::Color::Black;
    continueButtonText.setFont(font);
    continueButtonText.setCharacterSize(fontSize);
    continueButtonText.setFillColor(textColor);
    continueButtonText.setString("Continue to \n Memory Game");
}

void Level1Screen::setupContinueToPongButton()
{
    sf::Vector2f buttonSize(200.0f, 60.0f);
    continueToPongButton.setSize(buttonSize);
    continueToPongButton.setFillColor(sf::Color::Green);

    unsigned int fontSize = 30;
    sf::Color textColor = sf::Color::Black;
    continueToPongButtonText.setFont(font);
    continueToPongButtonText.setCharacterSize(fontSize);
    continueToPongButtonText.setFillColor(textColor);
    continueToPongButtonText.setString("Continue to \n MathQuest Pong");
}

void Level1Screen::handleQuestionEvent(const sf::Event& event)
{
    if (event.text.unicode >= 48 && event.text.unicode <= 57) // Numeric characters 0-9
    {
        answerInput += static_cast<char>(event.text.unicode);
        answerText.setString(answerInput);
    }
    else if (event.text.unicode == 8 && !answerInput.empty())  // Backspace
    {
        answerInput.pop_back();
        answerText.setString(answerInput);
    }
}

void Level1Screen::handleEnterButtonEvent(const sf::Event& event)
{
    if (!gameOver) {
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        sf::Vector2f worldPosition = window.mapPixelToCoords(mousePosition);
        if (enterButton.getGlobalBounds().contains(worldPosition)) {
            // Play the click sound
            clickSound.play();

            // Check the user's answer
            if (answerInput == questionsAndAnswers[currentQuestionIndex].second) {
                correctAnswerSound.play();
                score += 100;
                scoreText.setString("Your Score: \n       " + formatScore(score));
            }
            else {
                wrongAnswerSound.play();
            }

            // Reset the answer input
            answerInput.clear();
            answerText.setString(answerInput);

            // Display the next question or end the game
            if (questionsAnswered < 5) {
                displayNextQuestion();
            }
            else {
                gameOver = true;
                endGame();
            }
        }
    }
}

void Level1Screen::handleTryAgainButtonEvent(const sf::Event& event)
{
    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
    sf::Vector2f worldPosition = window.mapPixelToCoords(mousePosition);
    if (tryAgainButtonVisible && tryAgainButton.getGlobalBounds().contains(worldPosition)) {
        // Play the click sound
        clickSound.play();

        // Reset the game
        resetGame();
    }
    else if (tryAgainButtonVisible && continueButton.getGlobalBounds().contains(worldPosition)) {
        // Play the click sound
        clickSound.play();

        // Transition to the next screen (activeScreen set to 3)
        activeScreen = 3;
    }
}

void Level1Screen::handleContinueToPongButtonEvent(const sf::Event& event)
{
    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
    sf::Vector2f worldPosition = window.mapPixelToCoords(mousePosition);
    if (gameOver && continueToPongButton.getGlobalBounds().contains(worldPosition)) {
        // Play the click sound
        clickSound.play();

        // Transition to the MathQuest Pong screen (activeScreen set to 4)
        activeScreen = 4;
    }
}

void Level1Screen::displayNextQuestion()
{
    if (questionsAnswered >= 5) {
        gameOver = true;
        endGame();
        return;
    }

    // Generate a random index that hasn't been used before
    std::set<int> usedIndices;
    usedIndices.insert(currentQuestionIndex); // Add the current question index to the set

    // Find a new unique question index
    do {
        currentQuestionIndex = getRandomQuestionIndex();
    } while (usedIndices.count(currentQuestionIndex) > 0);

    usedIndices.insert(currentQuestionIndex); // Add the new question index to the set

    questionText.setString(questionsAndAnswers[currentQuestionIndex].first);
    questionsAnswered++;
}

void Level1Screen::endGame()
{
    // Hide unnecessary elements
    questionText.setString("");
    answerBox.setFillColor(sf::Color::Transparent);
    enterButton.setFillColor(sf::Color::Transparent);

    // Display end game messages and buttons
    if (score == 500) {
        titleText.setString("Congratulations! You passed Level 1");
    }
    else {
        titleText.setString("You did not pass Level 1!");
    }

    // Position the try again button
    tryAgainButton.setPosition(window.getSize().x / 3.5f - tryAgainButton.getSize().x / 2.f,
                               window.getSize().y / 2.f + 100.f);

    // Position the continue button
    continueButton.setPosition(window.getSize().x / 3.5f - continueButton.getSize().x / 2.f,
                               tryAgainButton.getPosition().y + tryAgainButton.getSize().y + 20.0f);

    // Position the continue to Pong button
    continueToPongButton.setPosition(window.getSize().x / 3.5f - continueToPongButton.getSize().x / 2.f,
                                     continueButton.getPosition().y + continueButton.getSize().y + 20.0f);

    // Center the try again button text within the button rectangle
    sf::FloatRect tryAgainButtonTextBounds = tryAgainButtonText.getLocalBounds();
    float tryAgainButtonTextX = tryAgainButton.getPosition().x + tryAgainButton.getSize().x / 2.f - tryAgainButtonTextBounds.width / 2.f;
    float tryAgainButtonTextY = tryAgainButton.getPosition().y + tryAgainButton.getSize().y / 2.f - tryAgainButtonTextBounds.height / 2.f;
    tryAgainButtonText.setPosition(tryAgainButtonTextX, tryAgainButtonTextY);

    // Center the continue button text within the button rectangle
    sf::FloatRect continueButtonTextBounds = continueButtonText.getLocalBounds();
    float continueButtonTextX = continueButton.getPosition().x + continueButton.getSize().x / 2.f - continueButtonTextBounds.width / 2.f;
    float continueButtonTextY = continueButton.getPosition().y + continueButton.getSize().y / 2.f - continueButtonTextBounds.height / 2.f;
    continueButtonText.setPosition(continueButtonTextX, continueButtonTextY);

    // Center the continue to Pong button text within the button rectangle
    sf::FloatRect continueToPongButtonTextBounds = continueToPongButtonText.getLocalBounds();
    float continueToPongButtonTextX = continueToPongButton.getPosition().x + continueToPongButton.getSize().x / 2.f - continueToPongButtonTextBounds.width / 2.f;
    float continueToPongButtonTextY = continueToPongButton.getPosition().y + continueToPongButton.getSize().y / 2.f - continueToPongButtonTextBounds.height / 2.f;
    continueToPongButtonText.setPosition(continueToPongButtonTextX, continueToPongButtonTextY);

    // Set the try again button visibility to true
    tryAgainButtonVisible = true;
}

void Level1Screen::resetGame()
{
    // Reset variables
    score = 0;
    currentQuestionIndex = -1;
    questionsAnswered = 0;
    answerInput.clear();
    tryAgainButtonVisible = false;
    gameOver = false;

    // Reset text and buttons
    titleText.setString("Level 1: Easy");
    scoreText.setString("Your Score:\n      000");
    questionText.setString("");
    answerBox.setFillColor(sf::Color::White);
    enterButton.setFillColor(sf::Color::Green);

    // Generate new questions and answers
    setupQuestionsAndAnswers();

    // Display the next question
    displayNextQuestion();
}

std::string Level1Screen::formatScore(int score)
{
    std::string scoreString = std::to_string(score);
    while (scoreString.length() < 3) {
        scoreString = "0" + scoreString;
    }
    return scoreString;
}
