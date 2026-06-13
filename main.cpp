#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 800}), "Battleship!");
    sf::RectangleShape rect({800.f, 400.f});
    sf::RectangleShape startButton({200.f, 100.f});
    rect.setFillColor(sf::Color::Red);
    sf::Font font("C:/Windows/Fonts/arial.ttf");
    sf::Text welcomeText(font);
    sf::Text buttonText(font);
    welcomeText.setString("Battleship");
    buttonText.setString("Start");
    welcomeText.setFillColor(sf::Color::Black);
    buttonText.setFillColor(sf::Color::Blue);
    welcomeText.setPosition({330.f, 230.f});
    buttonText.setPosition({365.f, 560.f});
    startButton.setFillColor(sf::Color::White);
    startButton.setPosition({300.f, 530.f});
    bool isOver = false;
    bool isClicked = false;
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }
        auto mousePosition = sf::Vector2f(sf::Mouse::getPosition(window));
        if (startButton.getGlobalBounds().contains(mousePosition)) {
            startButton.setFillColor(sf::Color::Blue);
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                if (!isClicked) {
                    startButton.setFillColor(sf::Color::White);
                    isClicked = true;
                }
            }
            else {
                startButton.setFillColor(sf::Color::Blue);
                isClicked = false;
            }
        }
        else {
            startButton.setFillColor(sf::Color::White);
        }
        window.clear();
        window.draw(rect);
        window.draw(welcomeText);
        window.draw(startButton);
        window.draw(buttonText);
        window.display();
    }
    return 0;
}