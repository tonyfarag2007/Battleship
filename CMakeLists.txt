cmake_minimum_required(VERSION 3.24)
project(Battleship_GUI)

set(CMAKE_CXX_STANDARD 17)

add_executable(Battleship_GUI main.cpp)

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR C:/SFML/lib/cmake/SFML)
find_package(SFML COMPONENTS system window graphics audio network REQUIRED)

include_directories(C:/SFML/include)
target_link_libraries(Battleship_GUI sfml-system sfml-window sfml-graphics sfml-audio)