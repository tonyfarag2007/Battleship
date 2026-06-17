#include <SFML/Graphics.hpp>
#include <iostream>
class Ship {
public:
    int length;
    int row, col;
    bool isVertical;
};
enum screen {
    WELCOME,
    GAME
};
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
    sf::Text playerOneText(font);
    playerOneText.setString("Player 1");
    sf::Text placeShipsText(font);
    placeShipsText.setString("Place Ships");
    bool isOver = false;
    bool isClicked = false;
    screen currentScreen = WELCOME;
    sf::RectangleShape boardOne[10][10];
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            boardOne[i][j].setSize({60.f, 60.f});
            boardOne[i][j].setPosition({30.f + j * 60.f, 100.f + i * 60.f});
            boardOne[i][j].setFillColor(sf::Color::Blue);
            boardOne[i][j].setOutlineColor(sf::Color::Black);
            boardOne[i][j].setOutlineThickness(2.f);
        }
    }
    Ship ships[5] = {
        {5, 0, 0, true},
        {4, 0, 0, true},
        {3, 0, 0, true},
        {2, 0, 0, true},
        {1, 0, 0, true}
    };
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
                    std::cout <<"Mouse click!" << std::endl;
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
            playerOneText.setPosition({275.f, 0.f});
            placeShipsText.setPosition({240.f, 750.f});
            sf::RectangleShape board({600.f, 600.f});
            sf::RectangleShape shipLoader({100.f, 750.f});
            shipLoader.setPosition({680.f, 25.f});
            board.setPosition({30.f, 100.f});
            window.draw(board);
            window.draw(playerOneText);
            window.draw(placeShipsText);
            window.draw(shipLoader);
            for (int i = 0; i < 10; i++) {
                for (int j = 0; j < 10; j++) {
                    window.draw(boardOne[i][j]);
                }
            }
        }
        window.display();
    }
    return 0;
}