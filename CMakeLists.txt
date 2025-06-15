# Specify the minimum required version of CMake
cmake_minimum_required(VERSION 3.31.6)

# Define the name of the project
project(MyGame LANGUAGES CXX)

# TODO: understand why we need these commands
find_package(asio REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(unofficial-libmysql REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(jwt-cpp CONFIG REQUIRED)

# Include the directory where the header files are located
include_directories(include)

# Collect all .cpp files from the src directory and store them in the SOURCES variable
# This will automatically add any new .cpp files added to the src/ folder
file(GLOB SOURCES src/*.cpp src/**/*.cpp src/**/**/*.cpp src/**/**/**/*.cpp)

# Create an executable target named 'mygame' from the collected .cpp files
# This will compile and link the source files to form the executable
add_executable(mygame ${SOURCES})

# TODO: understand why we need these commands
target_link_libraries(mygame PRIVATE 
    asio::asio
    nlohmann_json::nlohmann_json
    unofficial::libmysql::libmysql
    OpenSSL::SSL
    OpenSSL::Crypto
    jwt-cpp::jwt-cpp
    # wsock32 # for bug in Windows: https://stackoverflow.com/questions/38770895/how-to-fix-undefined-reference-to-getacceptexsockaddrs-boost-asio-in-clion
)

target_compile_features(mygame PRIVATE cxx_std_20)

# Set the C++ standard for the project (C++20 in this case)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++20") # for MSVC
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}") # for GCC/Clang