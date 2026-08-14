#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
void network() {
    sf::TcpSocket socket;
    std::optional<sf::IpAddress> myIp = sf::IpAddress::getLocalAddress();
    std::cout<<myIp->toString()<<std::endl;
}