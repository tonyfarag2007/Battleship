#include <SFML/Network.hpp>
#include <iostream>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Graphics.hpp>
#include "Network.h"
class Ship {
public:
    int length;
    int row, col;
    int cellsHit;
    bool isVertical, isDragged = false, isPlaced = false;
    sf::Vector2f dragOffset;
};
class Player {
public:
    Ship ships[5] = {
        {5, 0, 0, 0, true},
        {4, 0, 0, 0, true},
        {3, 0, 0, 0, true},
        {2, 0, 0, 0, true},
        {1, 0, 0, 0, true}
    };
    sf::RectangleShape board[10][10], trackingBoard[10][10], shipShapes[5];
    std::vector<std::vector<std::pair<int, int>>> shipLocations;
    int hitCount = 0;
    bool hasPassed = false;
    bool isReady = false;
};
Player playerOne, playerTwo;
enum screen {
    WELCOME,
    PLAYER_ONE,
    PLAYER_TWO,
    BATTLESHIP,
    GAME_OVER
};
enum winner {
    PLAYER_1,
    PLAYER_2
};
enum MessageType {
    ATTACK,
    HIT_RESULT,
    TURN_DONE,
    GAME_END
};
sf::Color customWhite(255, 255, 255, 180);
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
bool isOverlapping(std::vector<std::vector<std::pair<int, int>>> playerShipLocations, Ship ships[], int size) {
    for (int g = 0; g < 5; g++) {
        int shipIndex = g;
        for (int h = 0; h < 5; h++){
            if (h == shipIndex) continue;
            for (int i = 0; i < ships[g].length; i++) {
                for (int j = 0; j < ships[h].length; j++) {
                    if (playerShipLocations[g][i] == playerShipLocations[h][j]) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}
bool isHanging(std::vector<std::vector<std::pair<int, int>>> playerShipLocations, Ship ships[], int size) {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < ships[i].length; j++) {
            if (playerShipLocations[i][j].first > 9 || playerShipLocations[i][j].first < 0
                || playerShipLocations[i][j].second > 9 || playerShipLocations[i][j].second < 0) {
                return true;
            }
        }
    }
    return false;
}
bool placeShips(Player& player, sf::RenderWindow& window) {
    auto mousePosition = sf::Vector2f(sf::Mouse::getPosition(window));
    static bool wasLeftDown = false, wasRightDown = false;
            bool leftDown = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left), rightDown = sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);
            bool leftPressedNow = leftDown && !wasLeftDown, rightPressedNow = rightDown && !wasRightDown;
            bool releasedNow = !leftDown && wasLeftDown;
            if (rightPressedNow) {
                for (int i = 0; i < 5; i++) {
                    if (player.shipShapes[i].getGlobalBounds().contains(mousePosition)) {
                        player.ships[i].isVertical = !player.ships[i].isVertical;
                        if (!player.ships[i].isVertical) {
                            player.shipShapes[i].setSize({player.ships[i].length * 60.f, 60.f});
                        }
                        else {
                            player.shipShapes[i].setSize({60.f, player.ships[i].length * 60.f});
                        }
                        break;
                    }
                }
            }
            if (leftPressedNow) {
                for (int i = 0; i < 5; i++) {
                    if (player.shipShapes[i].getGlobalBounds().contains(mousePosition)) {
                        player.ships[i].isDragged = true;
                        player.ships[i].dragOffset = mousePosition - player.shipShapes[i].getPosition();
                        player.ships[i].isPlaced = false;
                        break;
                    }
                }
            }
            for (int i = 0; i < 5; i++) {
                if (player.ships[i].isDragged) {
                    player.shipShapes[i].setPosition(mousePosition - player.ships[i].dragOffset);
                }
            }
            if (releasedNow) {
                for (int i = 0; i < 5; i++) {
                    if (player.ships[i].isDragged) {
                        sf::Vector2f pos = player.shipShapes[i].getPosition();
                        bool insideBoard = (pos.x >= 600.f && pos.x <= 1200.f && pos.y >= 150.f && pos.y <= 750.f);
                        if (insideBoard) {
                            player.ships[i].col = static_cast<int>((pos.x - 600.f) / 60.f);
                            player.ships[i].row = static_cast<int>((pos.y - 150.f) / 60.f);
                            player.shipShapes[i].setPosition({600.f + player.ships[i].col * 60.f, 150.f + player.ships[i].row * 60.f});
                            player.ships[i].isPlaced = true;
                        }
                    }
                    player.ships[i].isDragged = false;
                }
            }
    wasLeftDown = leftDown;
    wasRightDown = rightDown;
            for (int i = 0; i < 5; i++) {
                if (!player.ships[i].isPlaced) {
                    return false;
                }
            }
    return true;
}
std::pair<int, int> attack(Player& player, sf::RenderWindow& window) {
    int rowHit = -1, colHit = -1;
    auto mousePosition = sf::Vector2f(sf::Mouse::getPosition(window));
    static bool wasLeftDown = false;
    bool leftDown = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
    bool leftPressedNow = leftDown && !wasLeftDown;
    bool hit = false;
                for (int i = 0; i<10; i++) {
                    for (int j = 0; j<10; j++) {
                            if (player.trackingBoard[i][j].getGlobalBounds().contains(mousePosition) &&
                                player.trackingBoard[i][j].getFillColor() != sf::Color::Red &&
                                player.trackingBoard[i][j].getFillColor() != customWhite &&
                                player.trackingBoard[i][j].getFillColor() != sf::Color::Cyan &&
                                player.trackingBoard[i][j].getFillColor() != sf::Color::White) {
                                player.trackingBoard[i][j].setFillColor(sf::Color::Yellow);
                                if(leftPressedNow) {
                                    sf::Vector2f hitLocation(mousePosition);
                                    colHit = static_cast<int>((hitLocation.x - 1120.f)/60.f);
                                    rowHit = static_cast<int>((hitLocation.y - 150.f)/60.f);
                                }
                            }
                            else if (!player.trackingBoard[i][j].getGlobalBounds().contains(mousePosition)
                                && player.trackingBoard[i][j].getFillColor() == sf::Color::Yellow) {
                                player.trackingBoard[i][j].setFillColor(sf::Color::Blue);
                                }
                    }
                }
    wasLeftDown = leftDown;
    return {rowHit, colHit};
}
bool handleOpponentAttack(Player& player, sf::RenderWindow& window, int rowHit, int colHit) {
    bool hitFound = false;
   for (int i = 0; i < 5; i++) {
       for (int j = 0; j<player.ships[i].length; j++) {
           if (player.shipLocations[i][j].first == rowHit && player.shipLocations[i][j].second == colHit) {
               player.board[rowHit][colHit].setFillColor(sf::Color::Red);
               hitFound = true;
           }
       }
   }
    if (!hitFound) {
        player.board[rowHit][colHit].setFillColor(sf::Color::White);
    }
    return hitFound;
}
bool playerWins(Player& player) {
    return player.hitCount == 15;
}
int main() {
    sf::RectangleShape rect({1920.f, 540.f});
    sf::RectangleShape startButton({200.f, 100.f});
    rect.setFillColor(sf::Color::Red);
    sf::Font font("C:/Windows/Fonts/arial.ttf");
    sf::Text welcomeText(font);
    sf::Text buttonText(font);
    sf::Text battleText(font);
    sf::Text gameOver(font);
    gameOver.setPosition({800.f, 540});
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
    sf::Text player2(font);
    player2.setString("Player 2");
    sf::Text placeShipsText(font);
    placeShipsText.setString("Place Ships");
    placeShipsText.setPosition({820.f, 800.f});
    sf::RectangleShape nextPlayer({180.f, 90.f});
    nextPlayer.setFillColor(sf::Color::Blue);
    nextPlayer.setPosition({1610.f, 750.f});
    sf::Text readyText(font);
    readyText.setString("Ready!");
    sf::Text battle(font);
    battle.setString("Battle!");
    battle.setPosition({1655.f, 775.f});
    readyText.setPosition({1650.f, 775.f});
    sf::RectangleShape shipLoader({200.f, 560.f});
    shipLoader.setPosition({1600.f, 160.f});
    sf::RectangleShape battleButton({180.f, 90.f});
    battleButton.setPosition({1610.f, 750.f});
    battleButton.setFillColor(sf::Color::Blue);
    sf::RectangleShape nextTurn({180.f, 90.f});
    nextTurn.setPosition({1400.f, 800.f});
    nextTurn.setFillColor(sf::Color::Yellow);
    bool isClicked = false;
    bool clicked = false;
    bool Clicked = false;
    bool isPlayerOneTurn = true, isPlayerTwoTurn = false;
    int winner;
    screen currentScreen = WELCOME;
    for (int i = 0; i < 5; i++) {
        playerOne.shipShapes[i].setSize({60.f, playerOne.ships[i].length * 60.f});
        playerTwo.shipShapes[i].setSize({60.f, playerTwo.ships[i].length * 60.f});
    }
    playerOne.shipShapes[0].setPosition({1620.f, 180.f});
    playerOne.shipShapes[1].setPosition({1720.f, 180.f});
    playerOne.shipShapes[2].setPosition({1720.f, 440.f});
    playerOne.shipShapes[3].setPosition({1620.f, 500.f});
    playerOne.shipShapes[4].setPosition({1620.f, 640.f});
    playerTwo.shipShapes[0].setPosition({1620.f, 180.f});
    playerTwo.shipShapes[1].setPosition({1720.f, 180.f});
    playerTwo.shipShapes[2].setPosition({1720.f, 440.f});
    playerTwo.shipShapes[3].setPosition({1620.f, 500.f});
    playerTwo.shipShapes[4].setPosition({1620.f, 640.f});
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            playerOne.board[i][j].setSize({60.f, 60.f});
            playerOne.board[i][j].setPosition({600.f + j * 60.f, 150.f + i * 60.f});
            playerOne.board[i][j].setFillColor(sf::Color::Blue);
            playerOne.board[i][j].setOutlineColor(sf::Color::Black);
            playerOne.board[i][j].setOutlineThickness(0.f);
            playerOne.trackingBoard[i][j].setSize({60.f, 60.f});
            playerOne.trackingBoard[i][j].setPosition({1120.f + j * 60.f, 150.f + i * 60.f});
            playerOne.trackingBoard[i][j].setFillColor(sf::Color::Blue);
            playerOne.trackingBoard[i][j].setOutlineColor(sf::Color::Black);
            playerOne.trackingBoard[i][j].setOutlineThickness(0.f);
            playerTwo.board[i][j].setSize({60.f, 60.f});
            playerTwo.board[i][j].setPosition({600.f + j * 60.f, 150.f + i * 60.f});
            playerTwo.board[i][j].setFillColor(sf::Color::Blue);
            playerTwo.board[i][j].setOutlineColor(sf::Color::Black);
            playerTwo.board[i][j].setOutlineThickness(0.f);
            playerTwo.trackingBoard[i][j].setSize({60.f, 60.f});
            playerTwo.trackingBoard[i][j].setPosition({1120.f + j * 60.f, 150.f + i * 60.f});
            playerTwo.trackingBoard[i][j].setFillColor(sf::Color::Blue);
            playerTwo.trackingBoard[i][j].setOutlineColor(sf::Color::Black);
            playerTwo.trackingBoard[i][j].setOutlineThickness(0.f);
        }
    }
    char connectionType;
    std::cout << "(s)erver or (c)lient?" << std::endl;
    std::cin >> connectionType;

    sf::TcpSocket socket;
    bool isConnected = false;
    if (connectionType == 's') {
        socket = hostGame();
        currentScreen = PLAYER_ONE;
        char buffer[128];
        std::size_t received;
        isConnected = true;
    }
    else if (connectionType == 'c') {
        currentScreen = PLAYER_TWO;
        std::string hostIp;
        std::cout << "Enter host IP: ";
        std::cin >> hostIp;
        socket = joinGame(hostIp);
        isConnected = true;
    }
    else {
        std::cout << "Invalid choice" << std::endl;
    }
     sf::Text text(font);
    if (connectionType == 'c') {
        text.setString("Hello Client!");
    }
    else if (connectionType == 's') {
        text.setString("Hello Server!");
    }
    if (isConnected) {
        sf::RenderWindow window(sf::VideoMode({1920, 1080}), "Battleship", sf::Style::Titlebar | sf::Style::Close);
        while (window.isOpen()) {
            while (const std::optional event = window.pollEvent()) {
                if (event->is<sf::Event::Closed>())
                    window.close();
            }
            auto mousePosition = sf::Vector2f(sf::Mouse::getPosition(window));
            window.clear();
            if (currentScreen == PLAYER_ONE) {
                playerOneText.setPosition({850.f, 30.f});
                window.draw(playerOneText);
                window.draw(placeShipsText);
                window.draw(shipLoader);
                for (int i = 0; i < 10; i++) {
                    for (int j = 0; j < 10; j++) {
                        window.draw(playerOne.board[i][j]);
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
                if (placeShips(playerOne, window)) {
                    playerOne.shipLocations = storeShipLocations(playerOne.ships, 5);
                    if (!isOverlapping(playerOne.shipLocations, playerOne.ships, 5) && !isHanging(playerOne.shipLocations, playerOne.ships, 5)) {
                        window.draw(nextPlayer);
                        window.draw(readyText);
                        if (nextPlayer.getGlobalBounds().contains(mousePosition)) {
                            nextPlayer.setFillColor(sf::Color::Green);
                            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                                if (!clicked && !playerOne.isReady) {
                                    clicked = true;
                                    std::cout<<"Click!"<<std::endl;
                                    playerOne.isReady = true;
                                    sf::Packet ready;
                                    ready << playerOne.isReady;
                                    socket.send(ready);
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
                }
                if (playerOne.isReady) {
                    sf::Packet received;
                    if (socket.receive(received) == sf::Socket::Status::Done) {
                        bool opponentReady = false;
                        received >> opponentReady;
                        if (opponentReady) {
                            currentScreen = BATTLESHIP;
                        }
                    }
                }
                for (int i = 0; i < 5; i++) {
                    playerOne.shipShapes[i].setFillColor(playerOne.shipShapes[i].getGlobalBounds().contains(mousePosition) ?
                        sf::Color(255, 0, 0, 150) : sf::Color(0, 255, 0, 150));
                }
                for (int i = 0; i < 5; i++) {
                    window.draw(playerOne.shipShapes[i]);
                }
            }
            else if (currentScreen == PLAYER_TWO) {
                player2.setPosition({850.f, 30.f});
                window.draw(player2);
                window.draw(placeShipsText);
                window.draw(shipLoader);
                for (int i = 0; i < 10; i++) {
                    for (int j = 0; j < 10; j++) {
                        window.draw(playerTwo.board[i][j]);
                    }
                }
                for (int j = 0; j <= 10; j++) {
                    sf::RectangleShape lineTwoVertical({1.f, 600.f});
                    lineTwoVertical.setFillColor(sf::Color::Black);
                    lineTwoVertical.setPosition({600.f + j * 60.f, 150.f});
                    window.draw(lineTwoVertical);
                }
                for (int i = 0; i <= 10; i++) {
                    sf::RectangleShape lineTwoHorizontal({600.f, 1.f});
                    lineTwoHorizontal.setFillColor(sf::Color::Black);
                    lineTwoHorizontal.setPosition({600.f, 150.f + i * 60.f});
                    window.draw(lineTwoHorizontal);
                }
                for (int i = 0; i < 5; i++) {
                    window.draw(playerTwo.shipShapes[i]);
                }
                if (placeShips(playerTwo, window)) {
                    playerTwo.shipLocations = storeShipLocations(playerTwo.ships, 5);
                    if (!isOverlapping(playerTwo.shipLocations, playerTwo.ships, 5)
                        && !isHanging(playerTwo.shipLocations, playerTwo.ships, 5)) {
                        window.draw(battleButton);
                        window.draw(readyText);
                        if (battleButton.getGlobalBounds().contains(mousePosition)) {
                            battleButton.setFillColor(sf::Color::Green);
                            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                                if (!Clicked && !playerTwo.isReady) {
                                    Clicked = true;
                                    std::cout<<"Click!"<<std::endl;
                                    playerTwo.isReady = true;
                                    sf::Packet ready;
                                    ready << playerTwo.isReady;
                                    socket.send(ready);
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
                }
                if (playerTwo.isReady) {
                    sf::Packet received;
                    if (socket.receive(received) == sf::Socket::Status::Done) {
                        bool opponentReady = false;
                        received >> opponentReady;
                        if (opponentReady) {
                            currentScreen = BATTLESHIP;
                        }
                    }
                }
                for (int i = 0; i < 5; i++) {
                    playerTwo.shipShapes[i].setFillColor(playerTwo.shipShapes[i].getGlobalBounds().contains(mousePosition) ?
                        sf::Color(255, 0, 0, 150) : sf::Color(0, 255, 0, 150));
                }
            }
            else if (currentScreen == BATTLESHIP) {
                if (connectionType == 's') {
                    playerOneText.setPosition({910.f, 800.f});
                    for (int i = 0; i < 10; i++) {
                        for (int j = 0; j < 10; j++) {
                            playerOne.board[i][j].setPosition({200.f + j * 60.f, 150.f + i * 60.f });
                            playerTwo.board[i][j].setPosition({200.f + j * 60.f, 150.f + i * 60.f });
                            window.draw(playerOne.board[i][j]);
                            window.draw(playerOne.trackingBoard[i][j]);
                        }
                    }
                    window.draw(battleText);
                    window.draw(playerOneText);
                    for (int j = 0; j <= 10; j++) {
                        sf::RectangleShape lineOneVertical({1.f, 600.f});
                        lineOneVertical.setFillColor(sf::Color::Black);
                        lineOneVertical.setPosition({1120.f + j * 60.f, 150.f});
                        window.draw(lineOneVertical);
                    }
                    for (int i = 0; i <= 10; i++) {
                        sf::RectangleShape lineOneHorizontal({600.f, 1.f});
                        lineOneHorizontal.setFillColor(sf::Color::Black);
                        lineOneHorizontal.setPosition({1120.f, 150.f + i * 60.f});
                        window.draw(lineOneHorizontal);
                    }
                    for (int j = 0; j <= 10; j++) {
                        sf::RectangleShape lineOneTrackingVertical({1.f, 600.f});
                        lineOneTrackingVertical.setFillColor(sf::Color::Black);
                        lineOneTrackingVertical.setPosition({200.f + j * 60.f, 150.f});
                        window.draw(lineOneTrackingVertical);
                    }
                    for (int i = 0; i <= 10; i++) {
                        sf::RectangleShape lineOneTrackingHorizontal({600.f, 1.f});
                        lineOneTrackingHorizontal.setFillColor(sf::Color::Black);
                        lineOneTrackingHorizontal.setPosition({200.f, 150.f + i * 60.f});
                        window.draw(lineOneTrackingHorizontal);
                    }
                    for (int i = 0; i < 5; i++) {
                        playerOne.shipShapes[i].setPosition({(600.f + playerOne.ships[i].col * 60.f) - 400.f, 150.f + playerOne.ships[i].row * 60.f});
                        playerTwo.shipShapes[i].setPosition({(600.f + playerTwo.ships[i].col * 60.f) - 400.f, 150.f + playerTwo.ships[i].row * 60.f});
                        window.draw(playerOne.shipShapes[i]);
                    }
                        sf::Packet incoming;
                        if (socket.receive(incoming) == sf::Socket::Status::Done) {
                            int message;
                            incoming >> message;
                            if (message == TURN_DONE) {
                                isPlayerOneTurn = true;
                            }
                            else if (message == ATTACK) {
                                sf::Packet receivedAttack;
                                int rowHit, colHit;
                                message = HIT_RESULT;
                                incoming >> rowHit >> colHit;
                                sf::Packet isHit;
                                isHit << message << handleOpponentAttack(playerOne, window, rowHit, colHit) << rowHit << colHit;
                                socket.send(isHit);
                            }
                            else if (message == HIT_RESULT) {
                                bool isHit;
                                int rowHit, colHit;
                                incoming >> isHit >> rowHit >> colHit;
                                if (isHit) {
                                    std::cout<<"Hit!"<<std::endl;
                                    std::cout<<rowHit<<":"<<colHit<<std::endl;
                                    playerOne.hitCount++;
                                    playerOne.trackingBoard[rowHit][colHit].setFillColor(sf::Color::Red);
                                    if (playerWins(playerOne)) {
                                        currentScreen = GAME_OVER;
                                        sf::Packet winDetected;
                                        winDetected << GAME_END;
                                        socket.send(winDetected);
                                    }
                                    else {
                                        isPlayerOneTurn = true;
                                    }
                                }
                                else if (!isHit) {
                                    std::cout<<"Miss!"<<std::endl;
                                    std::cout<<rowHit<<":"<<colHit<<std::endl;
                                    playerOne.trackingBoard[rowHit][colHit].setFillColor(sf::Color::White);
                                    message = TURN_DONE;
                                    sf::Packet playerTurnDone;
                                    playerTurnDone << message;
                                    socket.send(playerTurnDone);
                                }
                            }
                            if (message == GAME_END) {
                                currentScreen = GAME_OVER;
                            }
                        }
                    if (isPlayerOneTurn) {
                        std::pair<int, int> hitPair = attack(playerOne, window);
                        if (hitPair.first != -1 && hitPair.second != -1) {
                            sf::Packet rowAndColHit;
                            rowAndColHit << ATTACK << hitPair.first << hitPair.second;
                            socket.send(rowAndColHit);
                            isPlayerOneTurn = false;
                        }
                    }
                }
                else if (connectionType == 'c') {
                    player2.setPosition({910.f, 800.f});
                    for (int i = 0; i < 10; i++) {
                for (int j = 0; j < 10; j++) {
                    playerOne.board[i][j].setPosition({200.f + j * 60.f, 150.f + i * 60.f });
                    playerTwo.board[i][j].setPosition({200.f + j * 60.f, 150.f + i * 60.f });
                    window.draw(playerTwo.board[i][j]);
                    window.draw(playerTwo.trackingBoard[i][j]);
                }
            }
                    window.draw(battleText);
                    window.draw(player2);
            for (int j = 0; j <= 10; j++) {
                sf::RectangleShape lineOneVertical({1.f, 600.f});
                lineOneVertical.setFillColor(sf::Color::Black);
                lineOneVertical.setPosition({1120.f + j * 60.f, 150.f});
                window.draw(lineOneVertical);
            }
            for (int i = 0; i <= 10; i++) {
                sf::RectangleShape lineOneHorizontal({600.f, 1.f});
                lineOneHorizontal.setFillColor(sf::Color::Black);
                lineOneHorizontal.setPosition({1120.f, 150.f + i * 60.f});
                window.draw(lineOneHorizontal);
            }
            for (int j = 0; j <= 10; j++) {
                sf::RectangleShape lineOneTrackingVertical({1.f, 600.f});
                lineOneTrackingVertical.setFillColor(sf::Color::Black);
                lineOneTrackingVertical.setPosition({200.f + j * 60.f, 150.f});
                window.draw(lineOneTrackingVertical);
            }
            for (int i = 0; i <= 10; i++) {
                sf::RectangleShape lineOneTrackingHorizontal({600.f, 1.f});
                lineOneTrackingHorizontal.setFillColor(sf::Color::Black);
                lineOneTrackingHorizontal.setPosition({200.f, 150.f + i * 60.f});
                window.draw(lineOneTrackingHorizontal);
            }
            for (int i = 0; i < 5; i++) {
                playerTwo.shipShapes[i].setPosition({(600.f + playerTwo.ships[i].col * 60.f) - 400.f, 150.f + playerTwo.ships[i].row * 60.f});
                window.draw(playerTwo.shipShapes[i]);
            }
                    sf::Packet receivedAttack;
                    if (socket.receive(receivedAttack) == sf::Socket::Status::Done) {
                        int message;
                        receivedAttack >> message;
                        if (message == TURN_DONE) {
                            isPlayerTwoTurn = true;
                        }
                        else if (message == ATTACK) {
                            int rowHit, colHit;
                            receivedAttack >> rowHit >> colHit;
                            sf::Packet isHit;
                            message = HIT_RESULT;
                            isHit << message << handleOpponentAttack(playerTwo, window, rowHit, colHit) << rowHit << colHit;
                            socket.send(isHit);
                        }
                        else if (message == HIT_RESULT) {
                            bool isHit;
                            int rowHit, colHit;
                            receivedAttack >> isHit >> rowHit >> colHit;
                            if (isHit) {
                                std::cout<<"Hit!"<<std::endl;
                                std::cout<<rowHit<<":"<<colHit<<std::endl;
                                playerTwo.hitCount++;
                                playerTwo.trackingBoard[rowHit][colHit].setFillColor(sf::Color::Red);
                                if (playerWins(playerTwo)) {
                                    currentScreen = GAME_OVER;
                                    sf::Packet winDetected;
                                    winDetected << GAME_END;
                                    socket.send(winDetected);
                                }
                                else {
                                    isPlayerTwoTurn = true;
                                }
                            }
                            else if (!isHit) {
                                std::cout<<"Miss!"<<std::endl;
                                std::cout<<rowHit<<":"<<colHit<<std::endl;
                                playerTwo.trackingBoard[rowHit][colHit].setFillColor(sf::Color::White);
                                message = TURN_DONE;
                                sf::Packet playerTurnDone;
                                playerTurnDone << message;
                                socket.send(playerTurnDone);
                            }
                        }
                        else if (message == GAME_END) {
                            currentScreen = GAME_OVER;
                        }
                    }
                    if (isPlayerTwoTurn) {
                        std::pair <int, int> hitPair = attack(playerTwo, window);
                        if (hitPair.first != -1 && hitPair.second != -1) {
                            sf::Packet rowAndHitCol;
                            rowAndHitCol << ATTACK << hitPair.first << hitPair.second;
                            socket.send(rowAndHitCol);
                            isPlayerTwoTurn = false;
                        }
                    }
                }
            }
            else if (currentScreen == GAME_OVER) {
                gameOver.setString("Game Over!");
                window.draw(gameOver);
            }
            window.display();
        }
    }
    return 0;
}