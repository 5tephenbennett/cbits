# AI Coding Agent Instructions for CBits

## Project Overview

CBits is a comprehensive C99 bit manipulation library providing optimized operations for 8, 16, 32, and 64-bit integer types. The project emphasizes security, performance, and code quality through sophisticated build tooling, comprehensive testing, and runtime safety checks.

## Documentation Standards

### Professional Writing Guidelines

**CRITICAL: Maintain professional tone in all documentation**

1. **No Bragging or Superlatives**: Avoid terms like "100%", "exceptional", "amazing", "perfect", "complete", etc.
2. **No Achievement Language**: Do not use phrases like "ACHIEVEMENT", "✅", celebration emojis, or self-congratulatory language
3. **No Metric Bragging**: Do not emphasize specific numbers of functions, test counts, or coverage percentages
4. **Factual Descriptions**: Use neutral, factual language that describes capabilities without boasting
5. **Professional Tone**: Write as a technical reference, not marketing material

**Examples of Professional vs. Unprofessional Language:**

❌ Unprofessional: "100% API Coverage!", "Exceptional quality", "Complete test suite", "Perfect documentation"
✅ Professional: "API coverage verification", "Quality assurance tools", "Test suite", "API documentation"

❌ Unprofessional: "129+ Functions with complete coverage!"
✅ Professional: "Bit manipulation library with support for multiple data types"

❌ Unprofessional: "✅ ACHIEVEMENT: Perfect test coverage!"
✅ Professional: "Includes comprehensive testing tools"

### Code Documentation Standards

1. **Function Documentation**: Use standard Doxygen formatting with @code/@endcode blocks
2. **Example Code**: Provide clear, practical examples without claiming they are "perfect" or "complete"
3. **Technical Accuracy**: Focus on technical correctness over promotional language
4. **Consistent Formatting**: Maintain consistent documentation style across all modules

## Architecture & Code Organization

### Directory Structure
```
bit_ops/                    # Core library
├── include/               # Public API headers
│   ├── bit_ops_api.h     # Public API exports
│   ├── bit_ops.h         # Common macros/types
│   ├── bit8_ops.h        # 8-bit operations
│   ├── bit16_ops.h       # 16-bit operations
│   ├── bit32_ops.h       # 32-bit operations
│   └── bit64_ops.h       # 64-bit operations
├── src/                  # Implementation
│   ├── bit_ops.c         # Common code & lookup tables
│   ├── bit8_ops.c        # 8-bit implementations
│   ├── bit16_ops.c       # 16-bit implementations
│   ├── bit32_ops.c       # 32-bit implementations
│   └── bit64_ops.c       # 64-bit implementations
└── test/                 # Comprehensive test suite
    ├── gtest/            # Google Test framework
    └── interactive/      # Interactive CLI tools
```

### Key Design Patterns

1. **Type-Specific Modules**: Each bit width (8/16/32/64) has dedicated header and source files
2. **Consistent API**: All modules follow the same function naming pattern: `BIT{N}_{Operation}`
3. **Header/Source Separation**:
   - Headers contain inline functions and extern declarations
   - Source files contain lookup tables and complex implementations
4. **Lookup Table Optimization**: Shared `bits_set_table256` for fast bit counting operations

## Build System Architecture

### CMake Configuration

The project uses a sophisticated CMake setup with component-specific strategies:

#### Key CMake Variables
- `CMAKE_BUILD_TYPE`: Debug/Release (default: Release)
- `BUILD_TESTS`: Enable/disable test building (default: ON)
- `ENABLE_SANITIZERS_FOR_LIB`: Force sanitizers for library (default: auto)
- `ENABLE_SANITIZERS_RELEASE`: Enable sanitizers in release builds (default: OFF)

#### Sanitizer Strategy
```cmake
# Tests and interactive tools ALWAYS have sanitizers enabled
# Library has sanitizers enabled for Debug builds by default
# Release builds can optionally enable sanitizers via ENABLE_SANITIZERS_RELEASE
```

#### Compiler Flags (Unified across all targets)
```cmake
# Security & Safety
-fstack-protector-strong
-D_FORTIFY_SOURCE=2
-fPIE

# Comprehensive Warnings
-Wall -Wextra -Wpedantic
-Wformat=2 -Wformat-security
-Wnull-dereference -Wstack-protector
-Wtrampolines -Walloca
-Wvla -Warray-bounds=2
-Wimplicit-fallthrough=3
-Wtraditional-conversion
-Wshift-overflow=2
-Wcast-qual -Wstringop-overflow=4

# Enhanced Sanitizers (when enabled)
-fsanitize=address
-fsanitize=undefined
-fsanitize=leak
-fsanitize-address-use-after-scope
-fno-sanitize-recover=all
```

### Build Script (`build.sh`)

Comprehensive shell script with advanced options:

#### Essential Commands
```bash
# Basic builds
./build.sh                           # Release build with tests
./build.sh --debug                   # Debug build (sanitizers auto-enabled)
./build.sh --sanitizers             # Force sanitizers for library
./build.sh --sanitizers-release     # Enable sanitizers in release mode

# Development workflow
./build.sh --clean --debug           # Clean debug build
./build.sh --format                  # Format code with clang-format
./build.sh --format-check            # Check formatting
./build.sh --tidy                    # Run clang-tidy static analysis
./build.sh --docs                    # Generate API documentation with Doxygen

# Coverage analysis
./build.sh --debug --coverage        # Enable gcov coverage analysis
./scripts/check_coverage.sh          # API coverage analysis

# Production builds
./build.sh --release --no-tests      # Library-only release build
./build.sh --install --install-prefix /opt/cbits  # Install to custom location
```

## Development Workflows

### Code Quality Standards

1. **C99 Standard Compliance**: All code must compile with `-std=c99`
2. **No Goto Statements**: Use structured error handling with early returns
3. **Unsigned Arithmetic**: Use unsigned types for bit operations to avoid undefined behavior
4. **Proper Header Organization**:
   - Extern declarations in headers
   - Definitions in source files
   - Avoid static data in headers

### Testing Strategy

The project has three testing approaches with comprehensive coverage requirements:

1. **Google Test** (`test/gtest/`): C++ test framework with comprehensive coverage
2. **Interactive Tests** (`test/interactive/`): CLI tools for manual testing

#### Coverage Requirements

The project maintains API testing coverage:
- **BIT8**: Functions tested
- **BIT16**: Functions tested
- **BIT32**: Functions tested
- **BIT64**: Functions tested

**Total: API functions with testing coverage**
**Test Suite: Automated tests**

** CRITICAL: All new API functions MUST include testing before merge**

#### Running Tests & Coverage Analysis
```bash
# All tests (automatic with build)
./build.sh

# Build with coverage analysis
./build.sh --debug --coverage
./build/bit_ops/test/bit_ops_gtest
./scripts/check_coverage.sh

# Specific test frameworks
cd build && ctest -R gtest
cd build && ./test/interactive/bit_interactive
```

#### Coverage Analysis Tools

Use the provided coverage analysis script to ensure API testing:

```bash
# Analyze API coverage for all bit widths
./scripts/check_coverage.sh

# Install lcov for HTML coverage reports
sudo apt-get install lcov

# Generate detailed coverage reports
./build.sh --debug --coverage
./build/bit_ops/test/bit_ops_gtest
./scripts/check_coverage.sh
```

### Security & Runtime Safety

#### Sanitizer Configuration
- **AddressSanitizer**: Memory error detection (buffer overflows, use-after-free)
- **UndefinedBehaviorSanitizer**: Detects undefined behavior (signed overflow, invalid shifts)
- **LeakSanitizer**: Memory leak detection
- **Enhanced Features**: Use-after-scope detection, ODR violation checking

#### Common Pitfalls to Avoid
1. **Signed Integer Overflow**: Use unsigned types for bit manipulation
2. **Invalid Bit Shifts**: Ensure shift counts are within valid ranges
3. **Buffer Overflows**: Use proper bounds checking
4. **Multiple Definition Errors**: Avoid static data in headers

## Common Operations & Patterns

### Adding New Bit Operations

**REQUIRED PROCESS: All API additions require comprehensive testing**

1. **Function Naming**: Follow `BIT{N}_{Operation}` pattern
2. **Header Declaration**: Add to appropriate `bit{N}_ops.h`
3. **Implementation**: Add to `bit{N}_ops.c`
4. **Tests**: **MUST** add to gtest framework
5. **Coverage Verification**: Run `./scripts/check_coverage.sh` to verify testing
6. **Documentation**: Update function comments

#### Test-Driven Development Workflow

```bash
# 1. Write tests first for new API function
# Edit bit_ops/test/gtest/bit{N}_gtest.cpp

# 2. Implement the function
# Edit bit_ops/include/bit{N}_ops.h (declaration)
# Edit bit_ops/src/bit{N}_ops.c (implementation)

# 3. Verify coverage
./build.sh --debug --coverage
./build/bit_ops/test/bit_ops_gtest
./scripts/check_coverage.sh

# 4. Verify function coverage
```

### Lookup Table Usage

For operations requiring bit counting:
```c
// In header (bit_ops.h)
extern const unsigned char bits_set_table256[256];

// Usage in implementation
int count = bits_set_table256[value & 0xFF];
```

### Error Handling Pattern

Use structured error handling without goto:
```c
int function_name(void) {
    if (error_condition) {
        cleanup_if_needed();
        return ERROR_CODE;
    }

    // Main logic here

    return SUCCESS;
}
```

## IDE Integration & Tasks

### VS Code Tasks Available
- **Build (Release)**: Standard release build with tests
- **Build (Debug)**: Debug build with sanitizers enabled
- **Run Tests**: Execute test suite
- **Format Code**: Format source code with clang-format
- **Static Analysis**: Run clang-tidy analysis
- **Generate Documentation**: Create API docs with Doxygen
- **Clean Build**: Remove and rebuild all artifacts

### IDE Support Features

#### Compile Commands Database
The build system automatically generates `compile_commands.json` for enhanced IDE support:
- **IntelliSense**: Better code completion and error detection
- **Code Navigation**: Accurate go-to-definition and find-references
- **Static Analysis**: IDEs can understand all compile flags and definitions
- **Auto-generation**: Updated automatically after each build via `./build.sh`

The file includes all:
- Compiler flags (sanitizers, warnings, optimizations)
- Include paths (`bit_ops/include`, `bit_ops/src`)
- Preprocessor definitions (`ARCH_TWOS_COMPLEMENT`, `COUNT_BITS_LOOKUP`, `SIGNED_SHIFTS`, etc.)

### Debugging Configuration

For debugging with sanitizers:
```bash
# Build with debug info and sanitizers
./build.sh --debug

# Run with detailed sanitizer output
ASAN_OPTIONS=verbosity=3:halt_on_error=1 ./build/test/gtest/bit_ops_gtest
```

## Performance Considerations

1. **Inline Functions**: Small operations are inlined in headers
2. **Lookup Tables**: Used for complex bit counting operations
3. **Compiler Optimizations**: Release builds use `-O2` with security flags
4. **Benchmark Testing**: Interactive tools provide performance measurements

## Contribution Guidelines

### Before Making Changes
1. Run formatting check: `./build.sh --format-check`
2. Run static analysis: `./build.sh --tidy`
3. Build with sanitizers: `./build.sh --debug`
4. Run all tests: `./build.sh`

### Code Review Checklist
- [ ] Follows C99 standard
- [ ] Uses unsigned types for bit operations
- [ ] No goto statements
- [ ] Proper error handling
- [ ] **Tests added (mandatory)**
- [ ] **API coverage verified with `./scripts/check_coverage.sh`**
- [ ] Documentation updated
- [ ] Builds cleanly with all sanitizers

## Troubleshooting

### Common Build Issues

1. **Sanitizer Not Found**: Install clang/gcc development packages
2. **CMake Version**: Requires CMake 3.10+
3. **Compiler Compatibility**: Tested with GCC 7+ and Clang 6+

### Runtime Errors

1. **Undefined Behavior**: Enable UBSan with `--debug` builds
2. **Memory Issues**: Use AddressSanitizer for detection
3. **Performance Issues**: Profile with release builds and timing tools

## Quick Reference

### Essential Commands
```bash
# Development cycle
./build.sh --clean --debug           # Clean debug build with sanitizers
./build.sh --format                  # Format code
./build.sh --tidy                    # Static analysis
./build.sh --docs                    # Generate API documentation

# Coverage analysis
./build.sh --debug --coverage        # Build with coverage
./scripts/check_coverage.sh          # Analyze API coverage

# Production
./build.sh --release --no-tests      # Library-only build
./build.sh --install                 # Install library

# Testing
./build/bit_ops/test/bit_ops_gtest   # Test suite
./build/test/interactive/bit_interactive  # Interactive testing
ctest --output-on-failure            # Detailed test output
```

### Key Files to Understand
- `CMakeLists.txt`: Build configuration and sanitizer setup
- `build.sh`: Comprehensive build script with all options
- `scripts/check_coverage.sh`: API coverage analysis tool
- `bit_ops/include/bit_ops.h`: Core macros and shared definitions
- `bit_ops/src/bit_ops.c`: Lookup tables and common implementations
- `bit_ops/test/gtest/`: Google Test suite

This codebase prioritizes security, performance, and code quality. Always build with sanitizers during development and follow the established patterns for consistency and maintainability.