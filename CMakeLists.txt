cmake_minimum_required(VERSION 3.24)
project(Battleship_GUI)

set(CMAKE_CXX_STANDARD 17)

add_executable(Battleship_GUI main.cpp
        Network.cpp
        Network.h
)

add_executable(NetworkingTest
        NetworkingTest.cpp
        Network.cpp
        Network.h
)

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR C:/mingw64-sfml/SFML-3.1.0/lib/cmake/SFML)
find_package(SFML COMPONENTS Window Graphics Audio Network REQUIRED)

target_link_libraries(Battleship_GUI SFML::Window SFML::Graphics SFML::Audio SFML::Network)
target_link_libraries(NetworkingTest SFML::Window SFML::Graphics SFML::Audio SFML::Network)

# Statically link the MinGW runtime so the exe has no external DLL dependencies
target_link_options(Battleship_GUI PRIVATE -static -static-libgcc -static-libstdc++)
target_link_options(NetworkingTest PRIVATE -static -static-libgcc -static-libstdc++)