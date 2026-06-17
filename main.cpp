#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 800}), "Battleship");
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
    enum screen {
        WELCOME,
        GAME
    };
    screen currentScreen = WELCOME;
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
                    std::cout<<"Mouse click!" <<std::endl;
                    isClicked = true;
                    currentScreen = GAME;
                }
            }
            else {
                isClicked = false;
            }
        }
        else {
            startButton.setFillColor(sf::Color::White);
        }
        window.clear();
        if (currentScreen == WELCOME) {
            window.draw(rect);
            window.draw(welcomeText);
            window.draw(startButton);
            window.draw(buttonText);
        }
        else {
            sf::RectangleShape board({600.f, 600.f});
            board.setPosition({100.f, 100.f});
            window.draw(board);
            sf::RectangleShape grid({60.f, 60.f});
            grid.setFillColor(sf::Color::Blue);
            grid.setOutlineColor(sf::Color::Black);
            grid.setOutlineThickness(2.f);
            float xPosition = 100.f, yPosition = 40.f;
            for (int i = 0; i < 10; i++) {
                yPosition += 60.0;
                for (int j = 0; j < 10; j++) {
                    grid.setPosition({xPosition, yPosition});
                    window.draw(grid);
                    xPosition += 60.f;
                }
                xPosition = 100.f;
            }
        }
        window.display();
    }
    return 0;
}