#include <SFML/Network.hpp>
#include <iostream>

sf::TcpSocket hostGame() {
    sf::TcpListener listener;
    listener.listen(53000);

    std::optional<sf::IpAddress> myIp = sf::IpAddress::getLocalAddress();
    if (myIp.has_value()) {
        std::cout << "Hosting on: " << myIp->toString() << std::endl;
    }

    sf::TcpSocket socket;
    sf::Socket::Status status = listener.accept(socket);
    socket.setBlocking(false);
    if (status != sf::Socket::Status::Done) {
        std::cout << "Failed to accept connection" << std::endl;
    } else {
        std::cout << "Client connected!" << std::endl;
    }

    return socket;
}

sf::TcpSocket joinGame(const std::string& hostIp) {
    sf::TcpSocket socket;
    std::optional<sf::IpAddress> ip = sf::IpAddress::resolve(hostIp);
    if (!ip.has_value()) {
        std::cout << "Invalid IP" << std::endl;
        return socket;
    }

    sf::Socket::Status status = socket.connect(ip.value(), 53000);
    socket.setBlocking(false);
    if (status != sf::Socket::Status::Done) {
        std::cout << "Failed to connect" << std::endl;
    } else {
        std::cout << "Connected to host!" << std::endl;
    }

    return socket;
}
