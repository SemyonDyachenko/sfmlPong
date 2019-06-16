cmake_minimum_required(VERSION 3.14)
project(sfmlPong)

find_package(SFML 2.5 COMPONENTS audio graphics network)

set(CMAKE_CXX_STANDARD 14)

add_executable(sfmlPong main.cpp)

target_link_libraries(sfmlPong sfml-graphics sfml-audio sfml-network)