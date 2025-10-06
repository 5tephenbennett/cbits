# bit_ops Library

A C library providing bit manipulation operations for 8-bit, 16-bit, 32-bit, and optionally 64-bit data types.

Inspired by: https://graphics.stanford.edu/~seander/bithacks.html

## Building with CMake

### Prerequisites

- CMake 3.10 or higher
- A C compiler (GCC, Clang, or MSVC)

### Basic Build

```bash
mkdir build
cd build
cmake ..
make
```

### Compiler Support

The build system supports multiple C compilers with automatic detection and compiler-specific optimizations:

#### **GCC (GNU Compiler Collection)**
```bash
CC=gcc cmake ..
make
```
- Uses GCC-specific warnings: `-Wlogical-op`, `-Wduplicated-cond`, `-Wduplicated-branches`, `-Wnull-dereference`
- More verbose warning output for comprehensive code analysis

#### **Clang**
```bash
CC=clang cmake ..
make
```
- Uses Clang-specific warnings: `-Wconditional-uninitialized`, `-Wloop-analysis`, `-Wshift-sign-overflow`, `-Wtautological-compare`
- Cleaner output with more selective warnings, better understanding of intentional bit manipulation

#### **Apple Clang (macOS)**
```bash
CC=clang cmake ..  # Automatically detected as AppleClang
make
```
- Similar to Clang with macOS-specific optimizations

The CMake configuration automatically detects your compiler and applies appropriate warning levels and optimizations. All compilers use maximum warning levels (`-Wall -Wextra -Wpedantic -Werror`) while allowing necessary exceptions for bit manipulation operations.

### Warning and Error Handling

The build system uses aggressive warning levels to catch potential issues:

- **Maximum warning levels**: `-Wall -Wextra -Wpedantic`
- **Treat warnings as errors**: `-Werror` (with exceptions for bit manipulation)
- **Security hardening**: `-fstack-protector-strong`
- **Advanced static analysis**: Compiler-specific warnings for better code quality

**Allowed warnings** (bit manipulation specific):
- Shift operations on negative values (intentional bit hacks)
- Sign conversions in bit operations
- Shift count overflows (for cross-platform compatibility)
- Complex bit expression parentheses suggestions

This configuration ensures high code quality while allowing the low-level bit manipulation operations that are core to this library's functionality.

## Advanced Clang Features

When using Clang compiler, additional analysis and code quality tools are available:

### **Memory Safety and Static Analysis**
Enable with `-DENABLE_CLANG_ANALYSIS=ON`:

- **Address Sanitizer** - Detects buffer overflows, use-after-free, memory leaks
- **Undefined Behavior Sanitizer** - Catches signed integer overflow, null pointer dereference
- **Integer Sanitizer** - Detects signed/unsigned integer overflow
- **Nullability Sanitizer** - Validates null pointer annotations
- **Clang Static Analyzer** - Advanced static analysis for logic errors
- **Clang-Tidy** - Code quality and bug detection

**Usage:**
```bash
CC=clang cmake -DENABLE_CLANG_ANALYSIS=ON -DCMAKE_BUILD_TYPE=Debug ..
make
make static-analysis  # Run additional static analysis
```

### **Code Formatting**
Enable with `-DENABLE_CLANG_FORMAT=ON`:

- **clang-format** - Automatic code formatting preservation
- **Format checking** - Ensures consistent style

**Usage:**
```bash
CC=clang cmake -DENABLE_CLANG_FORMAT=ON ..
make format-check     # Check formatting consistency
make format          # Auto-format source code
```

### **Analysis Configuration Files**
- `.clang-format` - Preserves existing coding style
- `.clang-tidy` - Configures static analysis rules for bit manipulation safety

### Build Options

The following CMake options are available:

- `BUILD_SHARED_LIBS=ON/OFF` - Build shared libraries (default: ON)
- `BUILD_STATIC_LIBS=ON/OFF` - Build static libraries (default: ON)
- `BUILD_TESTS=ON/OFF` - Build test programs (default: OFF)
- `RUN_COMPILER_TESTS=ON/OFF` - Run compiler capability tests (default: ON)

The build system automatically detects:
- Platform type (Linux, Windows, etc.) and sets appropriate `LINUX_BUILD`/`WINDOWS_BUILD` definitions
- Native 64-bit integer support and sets `NATIVE_64BIT_UNSUPPORTED` if not available
- Whether to include 64-bit operations module based on availability and support

### Examples

Build only static libraries:
```bash
cmake -DBUILD_SHARED_LIBS=OFF -DBUILD_STATIC_LIBS=ON ..
```

Build with tests:
```bash
cmake -DBUILD_TESTS=ON ..
```

Build without compiler tests (faster but less accurate detection):
```bash
cmake -DRUN_COMPILER_TESTS=OFF ..
```

Build with specific compiler:
```bash
CC=clang cmake ..  # Use Clang
CC=gcc cmake ..    # Use GCC
```

Build in Debug mode with extra debugging info:
```bash
cmake -DCMAKE_BUILD_TYPE=Debug ..
```

Enable Clang advanced analysis (memory safety, static analysis):
```bash
CC=clang cmake -DENABLE_CLANG_ANALYSIS=ON -DCMAKE_BUILD_TYPE=Debug ..
```

Enable Clang formatting tools:
```bash
CC=clang cmake -DENABLE_CLANG_FORMAT=ON ..
```

### Installation

To install the library and headers:

```bash
make install
```

By default, this will install to `/usr/local`. You can change the installation prefix:

```bash
cmake -DCMAKE_INSTALL_PREFIX=/your/install/path ..
```

### Using the Library

After installation, you can use the library in other CMake projects:

```cmake
find_package(bit_ops REQUIRED)
target_link_libraries(your_target bit_ops::bit_ops)
```

Or link specifically to shared or static versions:

```cmake
target_link_libraries(your_target bit_ops::shared)  # For shared library
target_link_libraries(your_target bit_ops::static)  # For static library
```

## Library Features

- 8-bit, 16-bit, and 32-bit bit manipulation operations
- Optional 64-bit support (when available)
- Both shared and static library builds
- Cross-platform support (Linux, Windows, macOS)
- Multi-compiler support (GCC, Clang, Apple Clang)
- Maximum warning levels with compiler-specific optimizations
- Automatic platform and capability detection
- CMake integration for easy consumption