## How to run this project manually

### 1. Prerequisites

- C++20
- CMake
- vcpkg

### 2. Install dependencies

```bash
vcpkg install
```

### 3. Build project

```bash
mkdir build
cd build

cmake .. -DCMAKE_TOOLCHAIN_FILE="${YOUR_VCPKG_DIR}/scripts/buildsystems/vcpkg.cmake"
cmake --build .
```

### 4. Run project

```
./mygame
```

## How to run this project via docker

```
docker build -t <your-image-tag-name> .
```

```
docker run -p 8081:8081 <your-image-tag-name>
```

## What architecture this project uses

```
[Network Layer]       // TCP socket, WebSocket
        ⇅
[Protocol Layer]      // Encode/decode message packets
        ⇅
[Service Layer]       // Business logic
        ⇅
[Game World Layer]    // Game simulator
```
