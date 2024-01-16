#include "Memorygame.hpp"

MemoryGame::MemoryGame() : window(sf::VideoMode(1500, 1000), "Math Quest Memory Game") {
    // Load background image
    if (!backgroundTexture.loadFromFile("background.jpg")) {
        std::cout << "Failed to load background image." << std::endl;
         //You can add error handling code here if the image fails to load
    }
    backgroundSprite.setTexture(backgroundTexture);

    // Generate random numbers
    int a_rand = 2 + rand() % 5;
    int b_rand = 5 + rand() % 5;
    int c_rand = 10 + rand() % 5;

    // Question 1: factorial
    int factorial = 1;
    for (int i = 1; i <= b_rand; i++) {
        factorial *= i;
    }
    std::string ques_factorial = std::to_string(b_rand) + "!";
    std::string ans_factorial = std::to_string(factorial);

    // Question 2: derivative x^rand where x=2
    int expo = a_rand;
    int deriv_ans = b_rand * pow(2, b_rand - 1);
    //(d/dx)x^rand where x=2
    std::string ques_deriv = "(d/dx) of x^" + std::to_string(expo) + "\n where x=2";
    std::string ans_deriv = std::to_string(deriv_ans);

    // Question 3: area of a circle with radius c_rand
    int radius = b_rand;
    float circle_ans = pow(c_rand, 2) * 3.14;
    std::string ques_radius = "area of circle \n with radius " + std::to_string(radius);
    std::string ans_radius = std::to_string(circle_ans);

    // Question 4: hypotenuse of triangle with sides c_rand and a_rand (rounded)
    int side_one = a_rand;
    int side_two = b_rand;
    float hypot_ans = sqrt((pow(side_one, 2)) + (pow(side_two, 2)));
    std::string ques_hypo = " find hypotenuse \n of triangle \n with sides: " + std::to_string(side_one) + " , " + std::to_string(side_two);
    std::string ans_hypo = std::to_string(hypot_ans);

    // Question 5: sin^2(c_rand) + cos^2(b_rand)   answer is always 1
    std::string ques_sin = "sin^2(" + std::to_string(c_rand) + ") + cos^2(" + std::to_string(c_rand) + ")";
    std::string ans_sin = "1";

    // Question 6: (tan(c_rand) + cot(c_rand)) * 3     answer is always 3
    std::string ques_tan = "(tan(" + std::to_string(c_rand) + ") + cot(" + std::to_string(c_rand) + ")) * 3";
    std::string ans_tan = "3";

    // Question 7: exponent (2^a_rand)
    int exp = a_rand;
    int exp_ans = pow(2, exp);
    std::string ques_exp = "2^" + std::to_string(exp);
    std::string ans_exp = std::to_string(exp_ans);

    // Question 8: convert b_rand radians to degrees
    int rad = b_rand;
    float rad_ans = rad * (180 / 3.14);
    std::string ques_deg = "convert " + std::to_string(rad) + "\n radians to \n degrees";
    std::string ans_deg = std::to_string(rad_ans);

    std::vector<Pair> pairs = {
        {ques_factorial, ans_factorial},
        {ques_deriv, ans_deriv},
        {ques_radius, ans_radius},
        {ques_hypo, ans_hypo},
        {ques_sin, ans_sin},
        {ques_tan, ans_tan},
        {ques_exp, ans_exp},
        {ques_deg, ans_deg}
    };

    // Create question cards
    for (int i = 0; i < SIZE * SIZE / 2; ++i) {
        cards.push_back(createCard(pairs[i], true));
    }

    // Create answer cards
    for (int i = 0; i < SIZE * SIZE / 2; ++i) {
        cards.push_back(createCard(pairs[i], false));
    }

    // Shuffle after all cards have been created
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(cards.begin(), cards.end(), g);

    // Update positions of cards after shuffle
    float cardWidth = cards[0].rect.getSize().x * 1.1f;
    float cardHeight = cards[0].rect.getSize().y * 1.1f;
    float screenWidth = window.getSize().x;
    float screenHeight = window.getSize().y;

    float horizontalSpacing = ((screenWidth - (cardWidth * SIZE)) * 0.2f) / (SIZE + 1) * 1.80f;
    float verticalSpacing = ((screenHeight - (cardHeight * SIZE)) * 0.2f) / (SIZE + 1) * 1.80f;

    float posX = (screenWidth - (cardWidth * SIZE) - (horizontalSpacing * (SIZE - 1))) / 2.0f;
    float posY = (screenHeight - (cardHeight * SIZE) - (verticalSpacing * (SIZE - 1))) / 2.0f + 50.0f;

    for (int i = 0; i < SIZE * SIZE; ++i) {
        cards[i].rect.setSize(sf::Vector2f(cardWidth, cardHeight));
        cards[i].rect.setPosition(posX, posY);
        cards[i].flipButton.setPosition(posX + cardWidth / 2.0f - cards[i].flipButton.getSize().x / 2.0f,
            posY + cardHeight / 2.0f - cards[i].flipButton.getSize().y / 2.0f);
        sf::FloatRect textBounds = cards[i].buttonText.getLocalBounds();
        cards[i].buttonText.setOrigin(textBounds.left + textBounds.width / 2.0f,
            textBounds.top + textBounds.height / 2.0f);
        cards[i].buttonText.setPosition(cards[i].flipButton.getPosition().x + cards[i].flipButton.getSize().x / 2.0f,
            cards[i].flipButton.getPosition().y + cards[i].flipButton.getSize().y / 2.0f);

        posX += cardWidth + horizontalSpacing;
        if ((i + 1) % SIZE == 0) {
            posX = (screenWidth - (cardWidth * SIZE) - (horizontalSpacing * (SIZE - 1))) / 2.0f;
            posY += cardHeight + verticalSpacing;
        }
    }

    if (!font.loadFromFile("myfnt.ttf"))
        exit(EXIT_FAILURE);
    lastCard = -1;
    wait = false;
    showInstructions = true;
}

Card MemoryGame::createCard(Pair pair, bool isQuestion) {
    Card card;
    // Set card size and position
    card.rect.setSize(sf::Vector2f(280.f * 0.8f, 180.f * 0.8f));
    card.rect.setPosition(0.f, 0.f);

    // Set card appearance
    card.rect.setFillColor(sf::Color::Cyan);
    card.rect.setOutlineColor(sf::Color::Black);
    card.rect.setOutlineThickness(5.f);

    // Set card shadow effect
    sf::Vector2f shadowOffset(15.f, 15.f);
    card.rect.setFillColor(sf::Color(230, 230, 230));
    card.rect.setOutlineThickness(4.f);
    card.rect.setOutlineColor(sf::Color(150, 150, 150));
    card.rect.setPosition(shadowOffset);

    // Set card flip button
    card.flipButton.setSize(sf::Vector2f(120.f * 0.8f, 60.f * 0.8f));
    card.flipButton.setFillColor(sf::Color::Green);
    card.flipButton.setOutlineColor(sf::Color::Black);
    card.flipButton.setOutlineThickness(4.f);

    // Set card text
    card.pair = pair;
    card.text = isQuestion ? pair.question : pair.answer;
    card.revealed = false;
    card.matched = false;
    card.isQuestion = isQuestion;

    card.buttonText.setFont(font);
    card.buttonText.setString("Flip");
    card.buttonText.setCharacterSize(36);
    card.buttonText.setFillColor(sf::Color::Black);
    card.buttonText.setStyle(sf::Text::Bold);

    // Set card text position
    sf::FloatRect textBounds = card.buttonText.getLocalBounds();
    card.buttonText.setOrigin(textBounds.left + textBounds.width / 2.0f,
        textBounds.top + textBounds.height / 2.0f);
    card.buttonText.setPosition(card.flipButton.getPosition().x + card.flipButton.getSize().x / 2.0f,
        card.flipButton.getPosition().y + card.flipButton.getSize().y / 2.0f);

    return card;
}

int MemoryGame::countdown_timer(sf::RenderWindow& window, sf::Text countdown, int total_start) {
    std::time_t time = std::time(NULL);
    std::tm current_time = *std::localtime(&time);
    int curr_min = current_time.tm_min;
    int curr_sec = current_time.tm_sec;
    int total_curr = (curr_min * 60) + curr_sec;
    int change = total_curr - total_start;
    int time_left = 180 - change;
    int seconds = time_left % 60;
    int minutes = time_left / 60;
    if (seconds < 10) {
        countdown.setString("Time Left:\n     " + std::to_string(minutes) + ":0" + std::to_string(seconds));
    }
    else {
        countdown.setString("Time Left:\n     " + std::to_string(minutes) + ":" + std::to_string(seconds));
    }
    countdown.setPosition(1330.f, 65.f);
    window.draw(countdown);
    return time_left;
}

void MemoryGame::Run() {
    std::time_t time = std::time(NULL);
    std::tm start_time = *std::localtime(&time);
    backgroundSprite.setTexture(backgroundTexture);

    bool win = false;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed && !wait)
            {
                sf::Vector2i pos = sf::Mouse::getPosition(window);
                if (showInstructions) {
                    if (pos.x >= 0.1 * window.getSize().x && pos.x <= 0.9 * window.getSize().x &&
                        pos.y >= 0.1 * window.getSize().y && pos.y <= 0.9 * window.getSize().y) {
                        showInstructions = false;
                    }
                }
                else {
                    for (int i = 0; i < SIZE * SIZE; ++i) {
                        if (cards[i].flipButton.getGlobalBounds().contains(pos.x, pos.y) && !cards[i].matched) {
                            cards[i].revealed = !cards[i].revealed;
                            if (cards[i].revealed) {
                                cards[i].flipButton.setFillColor(sf::Color::White);  // Match the background
                                cards[i].flipButton.setOutlineThickness(0.f);  // Remove the outline
                            }
                            else {
                                cards[i].flipButton.setFillColor(sf::Color::Green);
                                cards[i].flipButton.setOutlineThickness(4.f);  // Restore the outline
                            }

                            if (lastCard != -1) {
                                if (cards[lastCard].pair.answer == cards[i].pair.answer) {
                                    cards[lastCard].matched = true;
                                    cards[i].matched = true;
                                    lastCard = -1;

                                    win = true;
                                    for (const Card& card : cards) {
                                        if (!card.matched) {
                                            win = false;
                                            break;
                                        }
                                    }
                                }
                                else {
                                    wait = true;
                                    clock.restart();
                                }
                            }
                            else {
                                lastCard = i;
                            }
                            break;
                        }
                    }
                }
            }
        }

        if (wait && clock.getElapsedTime() > sf::seconds(2.f)) {
            for (Card& card : cards) {
                if (!card.matched) {
                    card.revealed = false;
                    card.flipButton.setFillColor(sf::Color::Green);
                    card.flipButton.setOutlineThickness(4.f);  // Reset button color
                }
            }
            lastCard = -1;
            wait = false;
        }

        window.clear();

        // Calculate scale factors to fit the image
        float scaleX = static_cast<float>(window.getSize().x) / backgroundTexture.getSize().x;
        float scaleY = static_cast<float>(window.getSize().y) / backgroundTexture.getSize().y;
        float scaleFactor = std::max(scaleX, scaleY);

        // Set the scale and position of the background sprite
        backgroundSprite.setScale(scaleFactor, scaleFactor);
        backgroundSprite.setPosition(
            (window.getSize().x - backgroundSprite.getGlobalBounds().width) / 2.f,
            (window.getSize().y - backgroundSprite.getGlobalBounds().height) / 2.f);

        window.draw(backgroundSprite);

        sf::Text title;
        title.setFont(font);
        title.setFillColor(sf::Color::Black);
        title.setCharacterSize(40);
        title.setString("Math Quest Memory Game");

        sf::FloatRect titleBounds = title.getLocalBounds();

        sf::RectangleShape titleBackground;
        float padding = 10.0f;
        titleBackground.setSize(sf::Vector2f(titleBounds.width + 2 * padding, titleBounds.height + 2 * padding));
        titleBackground.setFillColor(sf::Color::Cyan);
        titleBackground.setPosition(
            (window.getSize().x - titleBackground.getSize().x) / 2.0f,
            50.f);
        title.setPosition(
            titleBackground.getPosition().x + (titleBackground.getSize().x - titleBounds.width) / 2.0f, 50.f);

        window.draw(titleBackground);
        window.draw(title);


        if (showInstructions) {
            sf::RectangleShape instructionsBox;
            instructionsBox.setSize(sf::Vector2f(window.getSize().x * 0.8f, window.getSize().y * 0.8f));
            instructionsBox.setFillColor(sf::Color::White);
            instructionsBox.setOutlineColor(sf::Color::Black);
            instructionsBox.setOutlineThickness(5.f);
            instructionsBox.setPosition(window.getSize().x * 0.1f, window.getSize().y * 0.1f);
            window.draw(instructionsBox);

            sf::Text instructions;
            instructions.setFont(font);
            instructions.setFillColor(sf::Color::Black);
            instructions.setCharacterSize(30);
            instructions.setString("Instructions:\n\nMatch the question cards with their corresponding answer cards.\n\nClick on a card to reveal its text. Try to remember the positions of the cards to make matches.\n\nOnce you match all the cards, the game is complete.\n\n");
            instructions.setPosition(instructionsBox.getPosition().x + 20.f, instructionsBox.getPosition().y + 20.f);
            window.draw(instructions);

            // Close button
            sf::Text closeButton;
            closeButton.setFont(font);
            closeButton.setFillColor(sf::Color::Black);
            closeButton.setCharacterSize(30);
            closeButton.setString("X");
            closeButton.setPosition(instructionsBox.getPosition().x + instructionsBox.getSize().x - 40.f, instructionsBox.getPosition().y + 10.f);
            window.draw(closeButton);
        }
        else {
            // Create countdown circle shape
            sf::CircleShape timerCircle(80.f);
            timerCircle.setFillColor(sf::Color::Transparent);
            timerCircle.setOutlineColor(sf::Color::Cyan);
            timerCircle.setOutlineThickness(10.f);
            timerCircle.setPosition(window.getSize().x - timerCircle.getRadius() * 2.f - 20.f, 20.f);

            // Create countdown text
            sf::Text countdown;
            countdown.setFont(font);
            countdown.setFillColor(sf::Color::White);
            countdown.setCharacterSize(30);

            // Update countdown timer
            int start_min = start_time.tm_min;
            int start_sec = start_time.tm_sec;
            int total_start = (start_min * 60) + start_sec;
            int time_left = countdown_timer(window, countdown, total_start);

            // Update countdown position
            sf::FloatRect countdownBounds = countdown.getLocalBounds();
            countdown.setOrigin(countdownBounds.width / 2.0f, countdownBounds.height / 2.0f);
            countdown.setPosition(
                timerCircle.getPosition() + sf::Vector2f(timerCircle.getRadius(), timerCircle.getRadius())
            );

            for (int i = 0; i < SIZE * SIZE; ++i) {
                window.draw(cards[i].rect);
                window.draw(cards[i].flipButton);
                if (!cards[i].revealed) {
                    window.draw(cards[i].buttonText);
                }
                if (cards[i].revealed || cards[i].matched) {
                    sf::Text text(cards[i].text, font);
                    text.setCharacterSize(25);
                    text.setFillColor(sf::Color::Black);
                    sf::FloatRect textRect = text.getLocalBounds();
                    text.setOrigin(textRect.left + textRect.width / 2.0f,
                        textRect.top + textRect.height / 2.0f);
                    text.setPosition(cards[i].rect.getPosition().x + cards[i].rect.getSize().x / 2.0f,
                        cards[i].rect.getPosition().y + cards[i].rect.getSize().y / 2.0f);
                    window.draw(text);
                }
            }

            // Draw countdown circle shape and text
            countdown.setPosition(1330.f, 65.f);
            window.draw(timerCircle);
            window.draw(countdown);

            if (time_left <= 0) {
                window.clear();
                sf::Text game_over;
                game_over.setFont(font);
                game_over.setFillColor(sf::Color::White);
                game_over.setPosition(window.getSize().x / 2.0f - game_over.getGlobalBounds().width / 2.0f, window.getSize().y / 2.0f - game_over.getGlobalBounds().height / 2.0f);
                game_over.setCharacterSize(50);
                game_over.setString("Game Over!");
                window.draw(game_over);
            }
        }
        if (win == true) {
            window.clear();
            if (!backgroundTexture.loadFromFile("background.jpg")) {
                std::cout << "Failed to load background image." << std::endl;
                //You can add error handling code here if the image fails to load
            }
            backgroundSprite.setTexture(backgroundTexture);
            sf::Text win_text;
            win_text.setFont(font); // add in your own 
            win_text.setCharacterSize(50);
            win_text.setFillColor(sf::Color::White);
            win_text.setString("You Win!");
            win_text.setPosition(window.getSize().x / 2.f - win_text.getGlobalBounds().width / 2.f, window.getSize().y / 2.f - win_text.getGlobalBounds().height / 2.f);
            window.draw(win_text);
        }
        window.display();
    }
}


