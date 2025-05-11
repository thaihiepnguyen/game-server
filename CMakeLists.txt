# Specify the minimum required version of CMake
cmake_minimum_required(VERSION 3.31.6)

# Define the name of the project
project(MyGame)

# TODO: understand why we need these commands
find_package(fmt REQUIRED)
find_package(asio REQUIRED)

# Collect all .cpp files from the src directory and store them in the SOURCES variable
# This will automatically add any new .cpp files added to the src/ folder
file(GLOB SOURCES src/*.cpp)

# Create an executable target named 'mygame' from the collected .cpp files
# This will compile and link the source files to form the executable
add_executable(mygame ${SOURCES})

# TODO: understand why we need these commands
target_link_libraries(mygame PRIVATE fmt::fmt)
target_link_libraries(mygame PRIVATE asio::asio)

# Set the C++ standard for the project (C++20 in this case)
set(CMAKE_CXX_STANDARD 20)