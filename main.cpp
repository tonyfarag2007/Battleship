#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
class Ship {
public:
    int length;
    int row, col;
    bool isVertical, isDragged = false, isPlaced = false;
    sf::Vector2f dragOffset;
};
enum screen {
    WELCOME,
    GAME
};
std::vector<std::vector<std::pair<int, int>>> storeShipLocations(Ship ships[], int size) {
    std::vector<std::vector<std::pair<int, int>>> shipLocations(size);
    for (int i = 0; i < size; i++) {
        shipLocations[i].push_back({ships[i].row, ships[i].col});
        }
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < ships[i].length - 1; j++) {
            if (ships[i].isVertical) {
                shipLocations[i].push_back({ships[i].row + (j+1), ships[i].col});
            }
            else if (!ships[i].isVertical) {
                shipLocations[i].push_back({ships[i].row, ships[i].col + (j+1)});
            }
        }
    }
    return shipLocations;
    }
int main() {
    sf::RenderWindow window(sf::VideoMode({1920, 1080}), "Battleship", sf::Style::Default);
    sf::RectangleShape rect({1920.f, 540.f});
    sf::RectangleShape startButton({200.f, 100.f});
    rect.setFillColor(sf::Color::Red);
    sf::Font font("C:/Windows/Fonts/arial.ttf");
    sf::Text welcomeText(font);
    sf::Text buttonText(font);
    welcomeText.setString("Battleship");
    buttonText.setString("Start");
    welcomeText.setFillColor(sf::Color::Black);
    buttonText.setFillColor(sf::Color::Blue);
    welcomeText.setPosition({850.f, 230.f});
    buttonText.setPosition({875.f, 700.f});
    startButton.setFillColor(sf::Color::White);
    startButton.setPosition({810.f, 670.f});
    sf::Text playerOneText(font);
    playerOneText.setString("Player 1");
    sf::Text placeShipsText(font);
    placeShipsText.setString("Place Ships");
    sf::RectangleShape nextPlayer({180.f, 90.f});
    nextPlayer.setFillColor(sf::Color::Blue);
    nextPlayer.setPosition({1610.f, 750.f});
    sf::Text playerTwoText(font);
    playerTwoText.setString("Player 2");
    playerTwoText.setPosition({1640.f, 775.f});
    bool isOver = false;
    bool isClicked = false;
    bool isPressed = false;
    screen currentScreen = WELCOME;
    sf::RectangleShape boardOne[10][10];
    Ship ships[5] = {
        {5, 0, 0, true},
        {4, 0, 0, true},
        {3, 0, 0, true},
        {2, 0, 0, true},
        {1, 0, 0, true}
    };
    std::vector<std::vector<std::pair<int, int>>> playerOneShipLocations;
    sf::RectangleShape shipShape[5];
    for (int i = 0; i < 5; i++) {
        shipShape[i].setSize({60.f, ships[i].length * 60.f});
    }
    shipShape[0].setPosition({1620.f, 180.f});
    shipShape[1].setPosition({1720.f, 180.f});
    shipShape[2].setPosition({1720.f, 440.f});
    shipShape[3].setPosition({1620.f, 500.f});
    shipShape[4].setPosition({1620.f, 640.f});
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            boardOne[i][j].setSize({60.f, 60.f});
            boardOne[i][j].setPosition({600.f + j * 60.f, 150.f + i * 60.f});
            boardOne[i][j].setFillColor(sf::Color::Blue);
            boardOne[i][j].setOutlineColor(sf::Color::Black);
            boardOne[i][j].setOutlineThickness(2.f);
        }
    }
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
            playerOneText.setPosition({850.f, 30.f});
            placeShipsText.setPosition({820.f, 800.f});
            sf::RectangleShape shipLoader({200.f, 560.f});
            shipLoader.setPosition({1600.f, 160.f});
            window.draw(playerOneText);
            window.draw(placeShipsText);
            window.draw(shipLoader);
            for (int i = 0; i < 10; i++) {
                for (int j = 0; j < 10; j++) {
                    window.draw(boardOne[i][j]);
                }
            }
            static bool wasLeftDown = false, wasRightDown = false;
            bool leftDown = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left), rightDown = sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);
            bool leftPressedNow = leftDown && !wasLeftDown, rightPressedNow = rightDown && !wasRightDown;
            bool releasedNow = !leftDown && wasLeftDown;
            if (rightPressedNow) {
                for (int i = 0; i < 5; i++) {
                    if (shipShape[i].getGlobalBounds().contains(mousePosition)) {
                        ships[i].isVertical = !ships[i].isVertical;
                        if (!ships[i].isVertical) {
                            shipShape[i].setSize({ships[i].length * 60.f, 60.f});
                        }
                        else {
                            shipShape[i].setSize({60.f, ships[i].length * 60.f});
                        }
                    }
                }
            }
            if (leftPressedNow) {
                for (int i = 0; i < 5; i++) {
                    if (shipShape[i].getGlobalBounds().contains(mousePosition)) {
                        ships[i].isDragged = true;
                        ships[i].dragOffset = mousePosition - shipShape[i].getPosition();
                        ships[i].isPlaced = false;
                        break;
                    }
                }
            }
            for (int i = 0; i < 5; i++) {
                if (ships[i].isDragged) {
                    shipShape[i].setPosition(mousePosition - ships[i].dragOffset);
                }
            }
            if (releasedNow) {
                for (int i = 0; i < 5; i++) {
                    if (ships[i].isDragged) {
                        sf::Vector2f pos = shipShape[i].getPosition();
                        bool insideBoard = (pos.x >= 600.f && pos.x <= 1200.f && pos.y >= 150.f && pos.y <= 750.f);
                        if (insideBoard) {
                            ships[i].col = static_cast<int>((pos.x - 600.f) / 60.f);
                            ships[i].row = static_cast<int>((pos.y - 150.f) / 60.f);
                            shipShape[i].setPosition({600.f + ships[i].col * 60.f, 150.f + ships[i].row * 60.f});
                            ships[i].isPlaced = true;
                        }
                    }
                    ships[i].isDragged = false;
                }
            }
            bool allShipsPlaced = true;
            for (int i = 0; i < 5; i++) {
                if (!ships[i].isPlaced) {
                    allShipsPlaced = false;
                    break;
                }
            }
            if (allShipsPlaced) {
                playerOneShipLocations = storeShipLocations(ships, 5);
                window.draw(nextPlayer);
                window.draw(playerTwoText);
            }
            for (int i = 0; i < 5; i++) {
                shipShape[i].setFillColor(shipShape[i].getGlobalBounds().contains(mousePosition) ? sf::Color::Red : sf::Color::Green);
            }
            wasLeftDown = leftDown;
            wasRightDown = rightDown;
            for (int i = 0; i < 5; i++) {
                window.draw(shipShape[i]);
            }
        }
        window.display();
    }
    return 0;
}