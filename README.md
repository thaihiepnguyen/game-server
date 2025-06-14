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

cmake .. -DCMAKE_TOOLCHAIN_FILE="${YOUR_VCPKG_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake"
cmake --build .
```

### 4. Run project

```
./mygame
```

### 5. Architecture

```
[Network Layer]       // TCP socket, WebSocket
        ⇅
[Protocol Layer]      // Encode/decode message packets
        ⇅
[Game Logic Layer]    // Session, combat, chat, world updates, etc.
        ⇅
[Service Layer]       // Business logic (optional layer)
        ⇅
[Repository Layer]           // Truy xuất dữ liệu từ DB
        ⇅
[Database]            // MySQL, PostgreSQL, Redis, etc.
```
