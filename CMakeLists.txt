cmake_minimum_required(VERSION 3.10)

project(MyGame)

file(GLOB SOURCES src/*.cpp)
add_executable(mygame ${SOURCES})

set(CMAKE_CXX_STANDARD 20)