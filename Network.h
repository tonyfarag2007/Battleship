//
// Created by tonyw on 2026-08-06.
//
#include <SFML/Network.hpp>

#ifndef BATTLESHIP_GUI_NETWORK_H
#define BATTLESHIP_GUI_NETWORK_H

sf::TcpSocket hostGame();
sf::TcpSocket joinGame(const std::string&);

#endif //BATTLESHIP_GUI_NETWORK_H