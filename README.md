# Game Server

A C++ game server implementation featuring real-time multiplayer capabilities with TCP networking and modular architecture.

## Table of Contents

- [Prerequisites](#prerequisites)
- [Installation](#installation)
- [Building the Project](#building-the-project)
- [Running the Application](#running-the-application)
- [Docker Deployment](#docker-deployment)
- [Architecture Overview](#architecture-overview)

## Prerequisites

Before building and running this project, ensure you have the following dependencies installed:

- **C++20 Compiler**: GCC 10+, Clang 12+, or MSVC 2019+
- **CMake**: Version 4.0 or higher
- **vcpkg**: Microsoft's C++ package manager
- **Git**: For version control

## Installation

### 1. Install Dependencies

Install required C++ libraries using vcpkg:

```bash
vcpkg isntall
```

**Note**: Ensure your vcpkg installation is properly configured and the `VCPKG_ROOT` environment variable is set.

## Building the Project

### Manual Build Process

1. Create a build directory:
   ```bash
   mkdir build
   cd build
   ```

2. Configure the project with CMake:
   ```bash
   cmake .. -DCMAKE_TOOLCHAIN_FILE="${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
   ```

3. Build the project:
   ```bash
   cmake --build . --config Release
   ```

## Running the Application

### Local Execution

After successful compilation, execute the server:

```bash
./mygame
```

## Docker Deployment

For containerized deployment, follow these steps:

### 1. Build Docker Image

```bash
docker build -t game-server:latest .
```

### 2. Run Container

```bash
docker run -d \
  --name game-server \
  -p 8081:8081 \
  game-server:latest
```

## Architecture Overview

The project follows a layered architecture pattern

```
┌─────────────────────┐
│   Network Layer     │  ← TCP connection communication, connection management
├─────────────────────┤
│   Protocol Layer    │  ← Message serialization, packet encoding/decoding
├─────────────────────┤
│   Command Layer     │  ← Packet categorization
├─────────────────────┤
│   Service Layer     │  ← Business logic, game session management
├─────────────────────┤
│   Game World Layer  │  ← Game state simulation, physics, collision detection
└─────────────────────┘
```

### Layer Responsibilities

- **Network Layer**: Handles low-level socket operations, client connections, and data transmission
- **Protocol Layer**: Manages message formatting, serialization, and protocol compliance
- **Command Layer**: Game commands (movement, attacks, room operations)
- **Service Layer**: Implements game business logic, player management, and session handling
- **Game World Layer**: Contains game simulation engine, physics calculations, and world state management


**Note**: This is just a practical project! It can't be ideal for production.