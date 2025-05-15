# Specify the minimum required version of CMake
cmake_minimum_required(VERSION 3.31.6)

# Define the name of the project
project(MyGame)

# TODO: understand why we need these commands
find_package(fmt REQUIRED)
find_package(asio REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
# find_package(mysql_connector_cpp CONFIG REQUIRED)

# Include the directory where the header files are located
include_directories(include)

# Collect all .cpp files from the src directory and store them in the SOURCES variable
# This will automatically add any new .cpp files added to the src/ folder
file(GLOB SOURCES src/*.cpp src/**/*.cpp)

# Create an executable target named 'mygame' from the collected .cpp files
# This will compile and link the source files to form the executable
add_executable(mygame ${SOURCES})

# TODO: understand why we need these commands
target_link_libraries(mygame PRIVATE fmt::fmt)
target_link_libraries(mygame PRIVATE asio::asio)
target_link_libraries(mygame PRIVATE nlohmann_json::nlohmann_json)
# target_link_libraries(mygame PRIVATE mysqlcppconn) TODO: check bug!

# Set the C++ standard for the project (C++20 in this case)
# set(CMAKE_CXX_STANDARD 20)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++20") # for MSVC
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20") # for GCC/Clang