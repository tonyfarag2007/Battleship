#include <SFML/Network.hpp>
#include <iostream>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Graphics.hpp>

#include "Network.h"

int main() {
    char connectionType;
    std::cout << "(s)erver or (c)lient?" << std::endl;
    std::cin >> connectionType;

    sf::TcpSocket socket;

    if (connectionType == 's') {
        socket = hostGame();
        char buffer[128];
        std::size_t received;
        sf::Socket::Status status = socket.receive(buffer, sizeof(buffer), received);
        if (status == sf::Socket::Status::Done) {
            std::cout << "Received: " << std::string(buffer, received) << std::endl;
        }
        else if (status == sf::Socket::Status::NotReady) {
            std::cout << "No data yet!" << std::endl;
        }
        else {
            std::cout << "Failed to receive data" << std::endl;
        }
    }
    else if (connectionType == 'c') {
        std::string hostIp;
        std::cout << "Enter host IP: ";
        std::cin >> hostIp;

        socket = joinGame(hostIp);

        // proof of connection: send one message to the server
        std::string msg = "hello from client";
        sf::Socket::Status status = socket.send(msg.c_str(), msg.size());
        if (status == sf::Socket::Status::Done) {
            std::cout << "Sent: " << msg << std::endl;
        }
        else if (status == sf::Socket::Status::NotReady) {
            std::cout << "No data yet!" << std::endl;
        }
        else {
            std::cout << "Failed to send data" << std::endl;
        }
    }
    else {
        std::cout << "Invalid choice" << std::endl;
    }
    sf::RenderWindow window(sf::VideoMode({1920, 1080}), "Battleship", sf::Style::Titlebar | sf::Style::Close);
     sf::Font font("C:/Windows/Fonts/arial.ttf");
     sf::Text text(font);
    if (connectionType == 'c') {
        text.setString("Hello Client!");
    }
    else if (connectionType == 's') {
        text.setString("Hello Server!");
    }

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }
        window.clear();
        window.draw(text);
        window.display();
    }



    return 0;
}