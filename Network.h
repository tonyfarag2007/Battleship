#include <SFML/Network.hpp>

#ifndef BATTLESHIP_GUI_NETWORK_H
#define BATTLESHIP_GUI_NETWORK_H

void startHosting(sf::TcpListener& listener);
sf::Socket::Status tryAccept(sf::TcpListener& listener, sf::TcpSocket& socket);

bool startJoining(sf::TcpSocket& socket, const std::string& hostIp);
sf::Socket::Status tryConnect(sf::TcpSocket& socket);

sf::Socket::Status connectToHost(sf::TcpSocket& socket, const sf::IpAddress& ip);

#endif //BATTLESHIP_GUI_NETWORK_H