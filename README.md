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
make
```