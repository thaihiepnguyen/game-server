cmake_minimum_required(VERSION 4.0)

project(MyGame LANGUAGES CXX)

find_package(asio CONFIG REQUIRED)

include_directories(include)

file(GLOB SOURCES src/*.cpp src/**/*.cpp src/**/**/*.cpp src/**/**/**/*.cpp)

add_executable(mygame ${SOURCES})

if(WIN32)
    target_link_libraries(mygame PRIVATE 
        asio::asio
        ws2_32
        wsock32
    )
elseif(APPLE)
    target_link_libraries(mygame PRIVATE 
        asio::asio
    )
elseif(UNIX)
    target_link_libraries(mygame PRIVATE 
        asio::asio
    )
endif()

target_compile_features(mygame PRIVATE cxx_std_20)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)