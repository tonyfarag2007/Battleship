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
    PLAYER_ONE,
    PLAYER_TWO,
    BATTLESHIP
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
    sf::Text battleText(font);
    battleText.setString("Battle!");
    battleText.setPosition({910.f, 30.f});
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
    sf::Text playerTwo(font);
    playerTwo.setString("Player 2");
    playerTwo.setPosition({850.f, 30.f});
    sf::Text placeShipsText(font);
    placeShipsText.setString("Place Ships");
    sf::RectangleShape nextPlayer({180.f, 90.f});
    nextPlayer.setFillColor(sf::Color::Blue);
    nextPlayer.setPosition({1610.f, 750.f});
    sf::Text playerTwoText(font);
    playerTwoText.setString("Player 2");
    sf::Text battle(font);
    battle.setString("Battle!");
    battle.setPosition({1655.f, 775.f});
    playerTwoText.setPosition({1640.f, 775.f});
    sf::RectangleShape shipLoader({200.f, 560.f});
    shipLoader.setPosition({1600.f, 160.f});
    sf::RectangleShape battleButton({180.f, 90.f});
    battleButton.setPosition({1610.f, 750.f});
    battleButton.setFillColor(sf::Color::Blue);
    sf::RectangleShape nextTurn({180.f, 90.f});
    nextTurn.setPosition({1400.f, 800.f});
    nextTurn.setFillColor(sf::Color::Yellow);
    bool isOver = false;
    bool isClicked = false;
    bool isPressed = false;
    bool clicked = false;
    bool Clicked = false;
    bool isSelected = false;
    bool isPlayerOneTurn = true, isPlayerTwoTurn = false;
    bool flag = false;
    screen currentScreen = WELCOME;
    sf::RectangleShape boardOne[10][10], boardTwo[10][10], boardOneTracking[10][10], boardTwoTracking[10][10];
    Ship playerOneShips[5] = {
        {5, 0, 0, true},
        {4, 0, 0, true},
        {3, 0, 0, true},
        {2, 0, 0, true},
        {1, 0, 0, true}
    };
    Ship playerTwoShips[5] = {
        {5, 0, 0, true},
        {4, 0, 0, true},
        {3, 0, 0, true},
        {2, 0, 0, true},
        {1, 0, 0, true}
    };
    std::vector<std::vector<std::pair<int, int>>> playerOneShipLocations, playerTwoShipLocations;
    sf::RectangleShape shipShapeOne[5], shipShapeTwo[5];
    sf::Vector2f shipOffset(400.f, 0.f);
    for (int i = 0; i < 5; i++) {
        shipShapeOne[i].setSize({60.f, playerOneShips[i].length * 60.f});
        shipShapeTwo[i].setSize({60.f, playerTwoShips[i].length * 60.f});
    }
    shipShapeOne[0].setPosition({1620.f, 180.f});
    shipShapeOne[1].setPosition({1720.f, 180.f});
    shipShapeOne[2].setPosition({1720.f, 440.f});
    shipShapeOne[3].setPosition({1620.f, 500.f});
    shipShapeOne[4].setPosition({1620.f, 640.f});
    shipShapeTwo[0].setPosition({1620.f, 180.f});
    shipShapeTwo[1].setPosition({1720.f, 180.f});
    shipShapeTwo[2].setPosition({1720.f, 440.f});
    shipShapeTwo[3].setPosition({1620.f, 500.f});
    shipShapeTwo[4].setPosition({1620.f, 640.f});
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            boardOne[i][j].setSize({60.f, 60.f});
            boardOne[i][j].setPosition({600.f + j * 60.f, 150.f + i * 60.f});
            boardOne[i][j].setFillColor(sf::Color::Blue);
            boardOne[i][j].setOutlineColor(sf::Color::Black);
            boardOne[i][j].setOutlineThickness(0.f);
            boardOneTracking[i][j].setSize({60.f, 60.f});
            boardOneTracking[i][j].setPosition({1120.f + j * 60.f, 150.f + i * 60.f});
            boardOneTracking[i][j].setFillColor(sf::Color::Blue);
            boardOneTracking[i][j].setOutlineColor(sf::Color::Black);
            boardOneTracking[i][j].setOutlineThickness(0.f);
            boardTwo[i][j].setSize({60.f, 60.f});
            boardTwo[i][j].setPosition({600.f + j * 60.f, 150.f + i * 60.f});
            boardTwo[i][j].setFillColor(sf::Color::Blue);
            boardTwo[i][j].setOutlineColor(sf::Color::Black);
            boardTwo[i][j].setOutlineThickness(0.f);
            boardTwoTracking[i][j].setSize({60.f, 60.f});
            boardTwoTracking[i][j].setPosition({500.f + j * 60.f, 150.f + i * 60.f});
            boardTwoTracking[i][j].setFillColor(sf::Color::Blue);
            boardTwoTracking[i][j].setOutlineColor(sf::Color::Black);
            boardTwoTracking[i][j].setOutlineThickness(0.f);
        }
    }
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }
        auto mousePosition = sf::Vector2f(sf::Mouse::getPosition(window));
        if (currentScreen == WELCOME) {
            if (startButton.getGlobalBounds().contains(mousePosition)) {
                startButton.setFillColor(sf::Color::Blue);
                if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                    if (!isClicked) {
                        std::cout <<"Mouse click!" << std::endl;
                        isClicked = true;
                        currentScreen = PLAYER_ONE;
                    }
                }
                else {
                    isClicked = false;
                }
            }
            else {
                startButton.setFillColor(sf::Color::White);
            }
        }
        if (currentScreen == WELCOME) {
            window.clear();
            window.draw(rect);
            window.draw(welcomeText);
            window.draw(startButton);
            window.draw(buttonText);
        }
        else if (currentScreen == PLAYER_ONE){
            window.clear();
            playerOneText.setPosition({850.f, 30.f});
            placeShipsText.setPosition({820.f, 800.f});
            window.draw(playerOneText);
            window.draw(placeShipsText);
            window.draw(shipLoader);
            for (int i = 0; i < 10; i++) {
                for (int j = 0; j < 10; j++) {
                    window.draw(boardOne[i][j]);
                }
            }
            for (int j = 0; j <= 10; j++) {
                sf::RectangleShape lineOne({1.f, 600.f});
                lineOne.setFillColor(sf::Color::Black);
                lineOne.setPosition({600.f + j * 60.f, 150.f});
                window.draw(lineOne);
            }
            for (int i = 0; i <= 10; i++) {
                sf::RectangleShape lineOne({600.f, 1.f});
                lineOne.setFillColor(sf::Color::Black);
                lineOne.setPosition({600.f, 150.f + i * 60.f});
                window.draw(lineOne);
            }
            static bool wasLeftDown = false, wasRightDown = false;
            bool leftDown = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left), rightDown = sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);
            bool leftPressedNow = leftDown && !wasLeftDown, rightPressedNow = rightDown && !wasRightDown;
            bool releasedNow = !leftDown && wasLeftDown;
            if (rightPressedNow) {
                for (int i = 0; i < 5; i++) {
                    if (shipShapeOne[i].getGlobalBounds().contains(mousePosition)) {
                        playerOneShips[i].isVertical = !playerOneShips[i].isVertical;
                        if (!playerOneShips[i].isVertical) {
                            shipShapeOne[i].setSize({playerOneShips[i].length * 60.f, 60.f});
                        }
                        else {
                            shipShapeOne[i].setSize({60.f, playerOneShips[i].length * 60.f});
                        }
                    }
                }
            }
            if (leftPressedNow) {
                for (int i = 0; i < 5; i++) {
                    if (shipShapeOne[i].getGlobalBounds().contains(mousePosition)) {
                        playerOneShips[i].isDragged = true;
                        playerOneShips[i].dragOffset = mousePosition - shipShapeOne[i].getPosition();
                        playerOneShips[i].isPlaced = false;
                        break;
                    }
                }
            }
            for (int i = 0; i < 5; i++) {
                if (playerOneShips[i].isDragged) {
                    shipShapeOne[i].setPosition(mousePosition - playerOneShips[i].dragOffset);
                }
            }
            if (releasedNow) {
                for (int i = 0; i < 5; i++) {
                    if (playerOneShips[i].isDragged) {
                        sf::Vector2f pos = shipShapeOne[i].getPosition();
                        bool insideBoard = (pos.x >= 600.f && pos.x <= 1200.f && pos.y >= 150.f && pos.y <= 750.f);
                        if (insideBoard) {
                            playerOneShips[i].col = static_cast<int>((pos.x - 600.f) / 60.f);
                            playerOneShips[i].row = static_cast<int>((pos.y - 150.f) / 60.f);
                            shipShapeOne[i].setPosition({600.f + playerOneShips[i].col * 60.f, 150.f + playerOneShips[i].row * 60.f});
                            playerOneShips[i].isPlaced = true;
                        }
                    }
                    playerOneShips[i].isDragged = false;
                }
            }
            bool allShipsPlaced = true;
            for (int i = 0; i < 5; i++) {
                if (!playerOneShips[i].isPlaced) {
                    allShipsPlaced = false;
                    break;
                }
            }
            if (allShipsPlaced) {
                playerOneShipLocations = storeShipLocations(playerOneShips, 5);
                window.draw(nextPlayer);
                window.draw(playerTwoText);
                if (nextPlayer.getGlobalBounds().contains(mousePosition)) {
                    nextPlayer.setFillColor(sf::Color::Green);
                    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                        if (!clicked) {
                            currentScreen = PLAYER_TWO;
                            clicked = true;
                            std::cout<<"Click!"<<std::endl;
                            window.clear();
                        }
                    }
                    else {
                        clicked = false;
                    }
                }
                else {
                    nextPlayer.setFillColor(sf::Color::Blue);
                }
            }
            for (int i = 0; i < 5; i++) {
                shipShapeOne[i].setFillColor(shipShapeOne[i].getGlobalBounds().contains(mousePosition) ? sf::Color::Red : sf::Color::Green);
            }
            wasLeftDown = leftDown;
            wasRightDown = rightDown;
            for (int i = 0; i < 5; i++) {
                window.draw(shipShapeOne[i]);
            }
        }
        else if (currentScreen == PLAYER_TWO){
            window.clear();
            static bool wasLeftDown = false, wasRightDown = false;
            bool leftDown = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left), rightDown = sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);
            bool leftPressedNow = leftDown && !wasLeftDown, rightPressedNow = rightDown && !wasRightDown;
            bool releasedNow = !leftDown && wasLeftDown;
            if (rightPressedNow) {
                for (int i = 0; i < 5; i++) {
                    if (shipShapeTwo[i].getGlobalBounds().contains(mousePosition)) {
                        playerTwoShips[i].isVertical = !playerTwoShips[i].isVertical;
                        if (!playerTwoShips[i].isVertical) {
                            shipShapeTwo[i].setSize({playerTwoShips[i].length * 60.f, 60.f});
                        }
                        else {
                            shipShapeTwo[i].setSize({60.f, playerTwoShips[i].length * 60.f});
                        }
                    }
                }
            }
            if (leftPressedNow) {
                for (int i = 0; i < 5; i++) {
                    if (shipShapeTwo[i].getGlobalBounds().contains(mousePosition)) {
                        playerTwoShips[i].isDragged = true;
                        playerTwoShips[i].dragOffset = mousePosition - shipShapeTwo[i].getPosition();
                        playerTwoShips[i].isPlaced = false;
                        break;
                    }
                }
            }
            for (int i = 0; i < 5; i++) {
                if (playerTwoShips[i].isDragged) {
                    shipShapeTwo[i].setPosition(mousePosition - playerTwoShips[i].dragOffset);
                }
            }
            if (releasedNow) {
                for (int i = 0; i < 5; i++) {
                    if (playerTwoShips[i].isDragged) {
                        sf::Vector2f pos = shipShapeTwo[i].getPosition();
                        bool insideBoard = (pos.x >= 600.f && pos.x <= 1200.f && pos.y >= 150.f && pos.y <= 750.f);
                        if (insideBoard) {
                            playerTwoShips[i].col = static_cast<int>((pos.x - 600.f) / 60.f);
                            playerTwoShips[i].row = static_cast<int>((pos.y - 150.f) / 60.f);
                            shipShapeTwo[i].setPosition({600.f + playerTwoShips[i].col * 60.f, 150.f + playerTwoShips[i].row * 60.f});
                            playerTwoShips[i].isPlaced = true;
                        }
                    }
                    playerTwoShips[i].isDragged = false;
                }
            }
            bool allShipsPlaced = true;
            for (int i = 0; i < 5; i++) {
                if (!playerTwoShips[i].isPlaced) {
                    allShipsPlaced = false;
                    break;
                }
            }
            if (allShipsPlaced) {
                playerTwoShipLocations = storeShipLocations(playerTwoShips, 5);
                window.draw(battleButton);
                window.draw(battle);
                if (battleButton.getGlobalBounds().contains(mousePosition)) {
                    battleButton.setFillColor(sf::Color::Green);
                    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                        if (!Clicked) {
                            Clicked = true;
                            std::cout<<"Click!"<<std::endl;
                            currentScreen = BATTLESHIP;
                        }
                    }
                    else {
                        Clicked = false;
                    }
                }
                else {
                    battleButton.setFillColor(sf::Color::Blue);
                }
            }
            window.draw(playerTwo);
            window.draw(placeShipsText);
            window.draw(shipLoader);
            for (int i = 0; i < 5; i++) {
                shipShapeTwo[i].setFillColor(shipShapeTwo[i].getGlobalBounds().contains(mousePosition) ? sf::Color::Red : sf::Color::Green);
            }
            wasLeftDown = leftDown;
            wasRightDown = rightDown;
            for (int i = 0; i < 10; i++) {
                for (int j = 0; j < 10; j++) {
                    window.draw(boardTwo[i][j]);
                }
            }
            for (int j = 0; j <= 10; j++) {
                sf::RectangleShape lineTwo({1.f, 600.f});
                lineTwo.setFillColor(sf::Color::Black);
                lineTwo.setPosition({600.f + j * 60.f, 150.f});
                window.draw(lineTwo);
            }
            for (int i = 0; i <= 10; i++) {
                sf::RectangleShape lineTwo({600.f, 1.f});
                lineTwo.setFillColor(sf::Color::Black);
                lineTwo.setPosition({600.f, 150.f + i * 60.f});
                window.draw(lineTwo);
            }
            for (int i = 0; i < 5; i++) {
                window.draw(shipShapeTwo[i]);
            }
        }
        else if (currentScreen == BATTLESHIP) {
            window.clear();
            static bool wasLeftDown = false, wasRightDown = false;
            bool leftDown = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left), rightDown = sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);
            bool leftPressedNow = leftDown && !wasLeftDown, rightPressedNow = rightDown && !wasRightDown;
            bool releasedNow = !leftDown && wasLeftDown;
            playerOneText.setPosition({910.f, 800.f});
            for (int i = 0; i < 10; i++) {
                for (int j = 0; j < 10; j++) {
                    boardOne[i][j].setPosition({200.f + j * 60.f, 150.f + i * 60.f });
                    window.draw(boardOne[i][j]);
                    window.draw(boardOneTracking[i][j]);
                }
            }
            if (isPlayerOneTurn) {
                for (int i = 0; i<10; i++) {
                    for (int j = 0; j<10; j++) {
                        if (boardOneTracking[i][j].getGlobalBounds().contains(mousePosition)) {
                            boardOneTracking[i][j].setFillColor(sf::Color::Red);
                            if(leftPressedNow) {
                                sf::Vector2f hitLocation(mousePosition);
                                int colHit = static_cast<int>((hitLocation.x - 1120.f)/60.f);
                                int rowHit = static_cast<int>((hitLocation.y - 150.f)/60.f);
                                for (int i = 0; i < 5 && !flag; i++) {
                                    for (int j = 0; j < playerTwoShips[i].length && !flag; j++) {
                                        if (playerTwoShipLocations[i][j].first == rowHit && playerTwoShipLocations[i][j].second == colHit) {
                                            std::cout<<"Player 2 Ship Hit!"<<std::endl;
                                            flag = true;
                                        }
                                    }
                                }
                                if (!flag) {
                                    std::cout<<"Miss!";
                                    flag = true;
                                }
                                isPlayerOneTurn = false;
                            }
                        }
                        else if (!boardOneTracking[i][j].getGlobalBounds().contains(mousePosition)){
                            boardOneTracking[i][j].setFillColor(sf::Color::Blue);
                        }
                    }
                }
            }
            window.draw(battleText);
            window.draw(playerOneText);
            for (int j = 0; j <= 10; j++) {
                sf::RectangleShape lineOne({1.f, 600.f});
                lineOne.setFillColor(sf::Color::Black);
                lineOne.setPosition({1120.f + j * 60.f, 150.f});
                window.draw(lineOne);
            }
            for (int i = 0; i <= 10; i++) {
                sf::RectangleShape lineOne({600.f, 1.f});
                lineOne.setFillColor(sf::Color::Black);
                lineOne.setPosition({1120.f, 150.f + i * 60.f});
                window.draw(lineOne);
            }
            for (int j = 0; j <= 10; j++) {
                sf::RectangleShape lineOne({1.f, 600.f});
                lineOne.setFillColor(sf::Color::Black);
                lineOne.setPosition({200.f + j * 60.f, 150.f});
                window.draw(lineOne);
            }
            for (int i = 0; i <= 10; i++) {
                sf::RectangleShape lineOneTracking({600.f, 1.f});
                lineOneTracking.setFillColor(sf::Color::Black);
                lineOneTracking.setPosition({200.f, 150.f + i * 60.f});
                window.draw(lineOneTracking);
            }
            for (int i = 0; i < 5; i++) {
                shipShapeOne[i].setPosition({(600.f + playerOneShips[i].col * 60.f) - 400.f, 150.f + playerOneShips[i].row * 60.f});
                window.draw(shipShapeOne[i]);
            }

        }
        window.display();
    }
    return 0;
}