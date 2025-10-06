# CBits
A collection of C libraries for bit manipulation

## Libraries

- **bit_ops**: Bit manipulation library with support for 8, 16, 32, and 64-bit operations
  - **Public API**: `#include "bit_ops_api.h"` - Single header for API access ([Documentation](docs/html/index.html))


### Building

```bash
# Simple build (Release mode with tests)
./build.sh

# Debug build with sanitizers
./build.sh --debug

# Debug build with coverage analysis
./build.sh --debug --coverage

# Clean build without tests
./build.sh --clean --no-tests

# Build and install
./build.sh --install

# Generate API documentation
./build.sh --docs

# Format code with clang-format
./build.sh --format

# Check code formatting
./build.sh --format-check

# Run static analysis
./build.sh --tidy
```

### Using CMake directly

```bash
mkdir build && cd build
cmake ..
cmake --build .
ctest  # Run tests
```

### Configuration Options

- `CMAKE_BUILD_TYPE`: Build type (Debug/Release, default: Release)
- `BUILD_SHARED_LIBS`: Build shared libraries (default: ON)
- `BUILD_STATIC_LIBS`: Build static libraries (default: ON)
- `BUILD_TESTS`: Build test programs (default: ON)
- `RUN_COMPILER_TESTS`: Run compiler capability tests (default: ON)
- `ENABLE_CLANG_ANALYSIS`: Enable Clang static analysis (default: ON)
- `ENABLE_CLANG_FORMAT`: Enable clang-format targets (default: ON)
- `ENABLE_SANITIZERS_FOR_LIB`: Force sanitizers for library (default: auto)
- `ENABLE_SANITIZERS_RELEASE`: Enable sanitizers in release builds (default: OFF)

Example:
```bash
cmake -DBUILD_TESTS=OFF -DBUILD_SHARED_LIBS=OFF ..
```

## Testing & Quality Assurance

The project includes testing and analysis tools:

- **Automated Testing**: Test suite using Google Test framework
- **Runtime Safety**: AddressSanitizer, UndefinedBehaviorSanitizer, LeakSanitizer
- **Coverage Analysis**: gcov integration with HTML coverage reports
- **Static Analysis**: clang-tidy integration for code quality
- **Interactive Testing**: CLI tools for manual testing and validation

Run tests with:
```bash
./build.sh              # Build and run all tests
./scripts/check_coverage.sh  # Analyze API coverage
```

### VS Code Coverage Integration

The project includes VS Code extensions for visual code coverage display:

**Installed Extensions:**
- **Coverage Gutters**: Shows line-by-line coverage in the editor gutter
- **Code Coverage**: Additional coverage analysis and visualization

**Quick Usage:**
1. Run the **"Generate Coverage"** task (Ctrl+Shift+P → "Tasks: Run Task")
2. Coverage will automatically display in the editor with:
   - ✅ **Green lines**: Covered by tests
   - ❌ **Red lines**: Not covered by tests
   - ➖ **No highlighting**: Non-executable lines

**VS Code Tasks Available:**
- `Generate Coverage`: Build with coverage, run tests, and generate coverage files
- `Build (Debug)`: Debug build with coverage enabled
- `Run Tests`: Execute the test suite

**Coverage Files Generated:**
- `coverage_filtered.info`: LCOV format covering only source files
- `lcov.info`: Standard LCOV filename for extension compatibility
- `build/coverage/html/`: HTML coverage report for browser viewing

The coverage extensions automatically watch for changes and update the display when you run tests.

## Documentation

- **API Documentation**: Doxygen documentation with examples
- **Code Examples**: Functions include formatted code examples
- **Generated Documentation**: HTML docs with search functionality
- **Coverage Analysis**: API coverage verification tools

Generate documentation:
```bash
./build.sh --docs       # Generate HTML documentation
```

## Project Structure

```
cbits/
├── bit_ops/           # Bit manipulation library
│   ├── include/       # Public headers
│   ├── src/           # Implementation
│   ├── test/          # Tests (gtest, interactive, unit)
│   └── CMakeLists.txt # Library-specific build config
├── docs/              # Generated API documentation
├── scripts/           # Build and analysis scripts
├── CMakeLists.txt     # Top-level build config
├── build.sh           # Build script
├── Doxyfile           # Documentation configuration
└── .clang-format      # Code formatting configuration
```

## Development

The project uses modern CMake practices with quality assurance tools:

### Code Quality Features
- **C99 Standard Compliance**: Strict adherence to C99 standard
- **Compiler Warnings**: Enhanced warning sets for all targets
- **Multiple Build Configurations**: Debug (with sanitizers) and Release modes
- **Sanitizer Integration**: Address, Undefined Behavior, and Leak sanitizers
- **Static Analysis**: clang-tidy integration with custom configuration
- **Code Formatting**: clang-format with project-specific style configuration
- **Platform Detection**: Automatic compiler capability testing

### Development Workflow
```bash
# Development cycle
./build.sh --clean --debug    # Clean debug build with sanitizers
./build.sh --format           # Format code
./build.sh --tidy             # Run static analysis
./build.sh --coverage         # Build with coverage analysis

# Verification
./scripts/check_coverage.sh   # Analyze API coverage
./build.sh --format-check     # Check code formatting
```

### Supported Compilers
- **GCC**: Version 7+ (recommended)
- **Clang**: Version 6+ (recommended)
- **Platform Support**: Linux, macOS (primary), Windows (via MinGW)

## Requirements

- CMake 3.10+
- C99-compatible compiler
- Optional: Google Test (for gtest suite)
- Optional: Clang tools (for static analysis and formatting)
- Optional: Doxygen (for documentation generation)
- Optional: gcov/lcov (for coverage analysis)