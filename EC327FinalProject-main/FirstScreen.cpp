#include "FirstScreen.h"
using namespace sf;

FirstScreen::FirstScreen(sf::RenderWindow& window, int& activeScreen)
    : window(window), activeScreen(activeScreen)
{
    // Load font for button text
    if (!font.loadFromFile("myfnt.ttf")) {
        std::cout << "Error: Failed to load font\n";
    }

    // Load background image
    if (!backgroundTexture.loadFromFile("background.png")) {
        std::cout << "Error: Failed to load image from file background.png\n";
    }
    else {
        // Scale the background image to fit the window size
        backgroundSprite.setTexture(backgroundTexture);
        float scaleX = static_cast<float>(window.getSize().x) / backgroundTexture.getSize().x;
        float scaleY = static_cast<float>(window.getSize().y) / backgroundTexture.getSize().y;
        backgroundSprite.setScale(scaleX, scaleY);
    }

    // Load click sound buffer
    if (!clickSoundBuffer.loadFromFile("click.wav")) {
        std::cout << "Error: Failed to load click sound\n";
    }

    // Load typing sound buffer
    if (!typingSoundBuffer.loadFromFile("typing.wav")) {
        std::cout << "Error: Failed to load typing sound\n";
    }

    // Create black rectangle at the top
    titleRectangle.setSize(sf::Vector2f(window.getSize().x, 100));
    titleRectangle.setPosition(0, 0);
    titleRectangle.setFillColor(sf::Color::Black);

    // Create title text
    titleText.setFont(font);
    titleText.setString("MathQuest");
    titleText.setCharacterSize(75);
    titleText.setFillColor(sf::Color::White);
    // Center the title text within the title rectangle
    sf::FloatRect textRect = titleText.getLocalBounds();
    titleText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    titleText.setPosition(sf::Vector2f(window.getSize().x / 2.0f, titleRectangle.getSize().y / 2.0f));

    // Create name label
    nameLabel.setFont(font);
    nameLabel.setString("Enter your name to continue:");
    nameLabel.setCharacterSize(60);
    nameLabel.setFillColor(sf::Color::Cyan);

    // Create name label rectangle
    nameLabelRectangle.setSize(sf::Vector2f(nameLabel.getLocalBounds().width + 30, nameLabel.getLocalBounds().height + 30));
    nameLabelRectangle.setPosition(window.getSize().x / 2 - nameLabelRectangle.getSize().x / 2, window.getSize().y / 2 - nameLabelRectangle.getSize().y / 2);
    nameLabelRectangle.setFillColor(sf::Color::Black);

    // Center the name label within the nameLabelRectangle
    sf::FloatRect nameLabelBounds = nameLabel.getLocalBounds();
    nameLabel.setOrigin(nameLabelBounds.left + nameLabelBounds.width / 2.0f, nameLabelBounds.top + nameLabelBounds.height / 2.0f);
    nameLabel.setPosition(sf::Vector2f(nameLabelRectangle.getPosition().x + nameLabelRectangle.getSize().x / 2.0f, nameLabelRectangle.getPosition().y + nameLabelRectangle.getSize().y / 2.0f));

    // Create name input rectangle
    nameInputRectangle.setSize(sf::Vector2f(300, nameLabel.getLocalBounds().height + 30));
    nameInputRectangle.setPosition(window.getSize().x / 2 - nameInputRectangle.getSize().x / 2, nameLabelRectangle.getPosition().y + nameLabelRectangle.getSize().y + 20);
    nameInputRectangle.setFillColor(sf::Color::Black);

    // Create name input text
    nameInputText.setFont(font);
    nameInputText.setString("");
    nameInputText.setCharacterSize(50);
    nameInputText.setFillColor(sf::Color::Cyan);
    nameInputText.setPosition(nameInputRectangle.getPosition().x + 10, nameInputRectangle.getPosition().y + 5);

    // Create button rectangle shape
    button.setSize(sf::Vector2f(270, 100));
    button.setPosition(window.getSize().x / 2 - button.getSize().x / 2, window.getSize().y - button.getSize().y - 70);
    button.setFillColor(sf::Color::Cyan); // Cyan color for the button

    // Create button text
    buttonText.setFont(font);
    buttonText.setString("Enter");
    buttonText.setCharacterSize(60);
    buttonText.setPosition(button.getPosition().x + button.getSize().x / 2 - buttonText.getLocalBounds().width / 2, button.getPosition().y + button.getSize().y / 2 - buttonText.getCharacterSize() / 2.0f);
    buttonText.setFillColor(sf::Color::White);
}

void FirstScreen::handleEvent(const sf::Event& event)
{
    if (event.type == sf::Event::MouseButtonReleased)
    {
        if (event.mouseButton.button == sf::Mouse::Left)
        {
            // Check if mouse click is inside the button
            sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
            if (button.getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y)))
            {
                // Check if the name field is not empty
                if (!nameInputText.getString().isEmpty())
                {
                    // Transition to the "Level 1: Easy" screen (activeScreen set to 2)
                    activeScreen = 2;

                    // Play click sound
                    clickSound.setBuffer(clickSoundBuffer);
                    clickSound.play();
                }
            }
        }
    }
    else if (event.type == sf::Event::TextEntered)
    {
        // Check if the entered character is valid
        if (event.text.unicode < 128 && event.text.unicode != '\b' && event.text.unicode != '\r' && event.text.unicode != '\n')
        {
            std::string currentString = nameInputText.getString();
            if (nameInputText.getGlobalBounds().width + 20 < nameInputRectangle.getSize().x)
            {
                currentString += static_cast<char>(event.text.unicode);
                nameInputText.setString(currentString);

                // Play typing sound
                typingSound.setBuffer(typingSoundBuffer);
                typingSound.play();
            }
        }
        else if (event.text.unicode == '\b' && !nameInputText.getString().isEmpty())
        {
            std::string currentString = nameInputText.getString();
            currentString.pop_back();
            nameInputText.setString(currentString);

            // Play typing sound
            typingSound.setBuffer(typingSoundBuffer);
            typingSound.play();
        }

        // After the text has been updated, reposition it within the nameInputRectangle
        sf::FloatRect textRect = nameInputText.getLocalBounds();
        nameInputText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        nameInputText.setPosition(sf::Vector2f(nameInputRectangle.getPosition().x + nameInputRectangle.getSize().x / 2.0f, nameInputRectangle.getPosition().y + nameInputRectangle.getSize().y / 2.0f));
    }
}

void FirstScreen::update()
{
    // Add any necessary update logic for the first screen here
}

void FirstScreen::render()
{
    // Draw the background sprite
    window.draw(backgroundSprite);

    // Draw the title rectangle and text
    window.draw(titleRectangle);
    window.draw(titleText);

    // Draw the name label rectangle, label, and input field
    window.draw(nameLabelRectangle);
    window.draw(nameLabel);
    window.draw(nameInputRectangle);
    window.draw(nameInputText);

    // Draw the button with enhanced appearance
    sf::Color buttonColor = sf::Color(30, 30, 30); // Darker color for the button
    if (button.getGlobalBounds().contains(static_cast<float>(sf::Mouse::getPosition(window).x), static_cast<float>(sf::Mouse::getPosition(window).y))) {
        // Apply lighter color when hovered over
        buttonColor = sf::Color(50, 50, 50);
    }
    button.setFillColor(buttonColor);
    button.setOutlineThickness(3);
    button.setOutlineColor(sf::Color::White);
    window.draw(button);
    window.draw(buttonText);
}