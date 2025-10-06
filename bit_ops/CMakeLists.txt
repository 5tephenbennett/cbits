cmake_minimum_required(VERSION 3.10)

# Project name and version
project(bit_ops VERSION 0.2 LANGUAGES C CXX)

# Generate compile_commands.json for IDE support
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Set default build type to Release if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Option to build shared libraries (default is shared)
option(BUILD_SHARED_LIBS "Build shared libraries" ON)
option(BUILD_STATIC_LIBS "Build static libraries" ON)
option(BUILD_TESTS "Build test programs" ON)
option(RUN_COMPILER_TESTS "Run compiler capability tests" ON)
option(ENABLE_CLANG_ANALYSIS "Enable Clang static analysis and sanitizers" ON)
option(ENABLE_CLANG_FORMAT "Enable clang-format style checking" ON)
option(ENABLE_SANITIZERS "Enable AddressSanitizer and other sanitizers for library (default: debug builds only)" OFF)
option(ENABLE_SANITIZERS_RELEASE "Enable AddressSanitizer for library in release builds too" OFF)
option(ENABLE_COVERAGE "Enable code coverage analysis with gcov/llvm-cov" OFF)

# Platform detection and compiler capability tests
if(RUN_COMPILER_TESTS)
    # Test for 64-bit integer support
    try_compile(HAVE_NATIVE_64BIT_SUPPORT
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../compiler_test/support_uint64.c
        OUTPUT_VARIABLE COMPILE_OUTPUT
    )

    # Test for signed 64-bit support
    try_compile(HAVE_NATIVE_INT64_SUPPORT
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../compiler_test/support_int64.c
        OUTPUT_VARIABLE COMPILE_OUTPUT
    )

    # Platform detection
    if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        set(LINUX_BUILD ON)
    endif()

    if(WIN32)
        set(WINDOWS_BUILD ON)
    endif()

else()
    # Fallback detection without compiler tests
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(HAVE_NATIVE_64BIT_SUPPORT ON)
        set(HAVE_NATIVE_INT64_SUPPORT ON)
    endif()

    if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        set(LINUX_BUILD ON)
    endif()

    if(WIN32)
        set(WINDOWS_BUILD ON)
    endif()
endif()

# Determine if we should mark 64-bit as unsupported
if(NOT HAVE_NATIVE_64BIT_SUPPORT OR NOT HAVE_NATIVE_INT64_SUPPORT)
    set(NATIVE_64BIT_UNSUPPORTED ON)
    message(STATUS "Native 64-bit support: DISABLED")
else()
    message(STATUS "Native 64-bit support: ENABLED")
endif()

# Determine sanitizer usage strategy
# Tests and interactive programs: Always enable sanitizers (if available)
set(ENABLE_SANITIZERS_FOR_TESTS ON)

# Library: Enable for debug builds by default, or if explicitly requested
if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR ENABLE_SANITIZERS_FOR_LIB)
    set(ENABLE_SANITIZERS_FOR_LIB ON)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release" AND ENABLE_SANITIZERS_RELEASE)
    set(ENABLE_SANITIZERS_FOR_LIB ON)
else()
    set(ENABLE_SANITIZERS_FOR_LIB OFF)
endif()

# Source files
set(BIT_OPS_SOURCES
    src/bit_ops.c
    src/bit8_ops.c
    src/bit16_ops.c
    src/bit32_ops.c
)

# Check for 64-bit operations module and add if 64-bit support is available
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/bit64_ops.c" AND NOT NATIVE_64BIT_UNSUPPORTED)
    set(HAVE_64_BIT_SUPPORT ON)
    list(APPEND BIT_OPS_SOURCES src/bit64_ops.c)
    message(STATUS "64-bit operations module: INCLUDED")
else()
    set(HAVE_64_BIT_SUPPORT OFF)
    message(STATUS "64-bit operations module: EXCLUDED")
endif()

# Include directories
set(BIT_OPS_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Common compile flags with high warning levels
set(BIT_OPS_COMPILE_FLAGS
    -Wall
    -Wextra
    -Wpedantic
    -Wcast-align
    -Wcast-qual
    -Wdisabled-optimization
    -Wformat=2
    -Winit-self
    -Wmissing-declarations
    -Wmissing-include-dirs
    -Wredundant-decls
    -Wshadow
    -Wstrict-overflow=5
    -Wswitch-default
    -Wundef
    -Wunreachable-code
    -Wunused
    -fPIC
    -fstack-protector-strong
    # Treat most warnings as errors, but allow specific bit manipulation warnings
    -Werror
    -Wno-error=shift-negative-value
    #-Wno-error=sign-conversion
    -Wno-error=shift-count-overflow
    #-Wno-error=parentheses
)

# Add compiler-specific flags
if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    list(APPEND BIT_OPS_COMPILE_FLAGS
        -Wlogical-op
        -Wduplicated-cond
        -Wduplicated-branches
        -Wnull-dereference
    )

    # Create enhanced sanitizer flags
    set(GCC_SANITIZER_FLAGS
        -fsanitize=address,undefined,leak,bounds-strict
        -fsanitize-address-use-after-scope
        -fno-omit-frame-pointer
        -fno-optimize-sibling-calls
        -fstack-protector-strong
        -fstack-clash-protection
        -fcf-protection=full
    )
    set(GCC_SANITIZER_LINK_FLAGS "-fsanitize=address,undefined,leak,bounds-strict")

    # Create coverage flags
    set(GCC_COVERAGE_FLAGS
        --coverage
        -fprofile-arcs
        -ftest-coverage
        -fno-omit-frame-pointer
    )
    set(GCC_COVERAGE_LINK_FLAGS "--coverage")

    # Enable coverage if requested
    if(ENABLE_COVERAGE)
        list(APPEND BIT_OPS_COMPILE_FLAGS ${GCC_COVERAGE_FLAGS})
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")
        message(STATUS "GCC coverage analysis: ENABLED")
    endif()

    # Enable sanitizers for library if requested
    if(ENABLE_SANITIZERS_FOR_LIB)
        list(APPEND BIT_OPS_COMPILE_FLAGS ${GCC_SANITIZER_FLAGS})
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GCC_SANITIZER_LINK_FLAGS}")
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${GCC_SANITIZER_LINK_FLAGS}")
        message(STATUS "GCC sanitizers for library: ENABLED (address, undefined, leak, bounds-strict + stack protection)")
    endif()

    message(STATUS "Using GCC-specific optimizations")
elseif(CMAKE_C_COMPILER_ID STREQUAL "Clang")
    list(APPEND BIT_OPS_COMPILE_FLAGS
        -Wconditional-uninitialized
        -Wloop-analysis
        -Wshift-sign-overflow
        -Wtautological-compare
        #-Wno-error=unknown-warning-option
    )

    # Enable advanced Clang features if requested
    if(ENABLE_CLANG_ANALYSIS)
        list(APPEND BIT_OPS_COMPILE_FLAGS
            # Static analysis warnings
            -Wthread-safety
            -Wthread-safety-beta
            -Wnullable-to-nonnull-conversion
            -Wnull-dereference
            -Warray-bounds-pointer-arithmetic
            -Wconditional-uninitialized
            -Wsometimes-uninitialized
            -Wstatic-in-inline
            -Wduplicate-enum
            -Wduplicate-method-match
            -Wfloat-equal
            -Wimplicit-fallthrough
            -Wcomma
            -Wformat-type-confusion
            -Widiomatic-parentheses
            -Wunreachable-code-aggressive
            -Wmove
        )

        message(STATUS "Clang advanced analysis: ENABLED")
    endif()

    # Create enhanced sanitizer flags
    set(CLANG_SANITIZER_FLAGS
        -fsanitize=address,undefined,integer,nullability,bounds
        -fsanitize-address-use-after-scope
        -fsanitize-address-use-odr-indicator
        -fno-omit-frame-pointer
        -fno-optimize-sibling-calls
        -fstack-protector-strong
    )
    set(CLANG_SANITIZER_LINK_FLAGS "-fsanitize=address,undefined,integer,nullability,bounds")

    # Enable sanitizers for library if requested
    if(ENABLE_SANITIZERS_FOR_LIB)
        list(APPEND BIT_OPS_COMPILE_FLAGS ${CLANG_SANITIZER_FLAGS})
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${CLANG_SANITIZER_LINK_FLAGS}")
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${CLANG_SANITIZER_LINK_FLAGS}")
        message(STATUS "Clang sanitizers for library: ENABLED (address, undefined, integer, nullability, bounds + stack protection)")
    endif()

    message(STATUS "Using Clang-specific optimizations")
elseif(CMAKE_C_COMPILER_ID STREQUAL "AppleClang")
    list(APPEND BIT_OPS_COMPILE_FLAGS
        -Wconditional-uninitialized
        -Wloop-analysis
        -Wtautological-compare
        -Wno-error=unknown-warning-option
    )
    message(STATUS "Using Apple Clang-specific optimizations")
endif()

# Define compile definitions based on platform and capability tests
set(BIT_OPS_COMPILE_DEFINITIONS)

if(LINUX_BUILD)
    list(APPEND BIT_OPS_COMPILE_DEFINITIONS LINUX_BUILD)
endif()

if(WINDOWS_BUILD)
    list(APPEND BIT_OPS_COMPILE_DEFINITIONS WINDOWS_BUILD)
endif()

if(NATIVE_64BIT_UNSUPPORTED)
    list(APPEND BIT_OPS_COMPILE_DEFINITIONS NATIVE_64BIT_UNSUPPORTED)
endif()

if(HAVE_64_BIT_SUPPORT)
    list(APPEND BIT_OPS_COMPILE_DEFINITIONS HAVE_64_BIT_SUPPORT)
endif()

# Enable COUNT_BITS_LOOKUP by default for optimized bit counting operations
list(APPEND BIT_OPS_COMPILE_DEFINITIONS COUNT_BITS_LOOKUP)

# Test for two's complement arithmetic (should be true on virtually all modern platforms)
if(RUN_COMPILER_TESTS)
    try_run(TWOS_COMPLEMENT_TEST_RUN TWOS_COMPLEMENT_TEST_COMPILE
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/test_twos_complement.c
        RUN_OUTPUT_VARIABLE TWOS_COMPLEMENT_OUTPUT
    )
    if(TWOS_COMPLEMENT_TEST_COMPILE AND TWOS_COMPLEMENT_TEST_RUN EQUAL 0)
        set(ARCH_TWOS_COMPLEMENT ON)
        message(STATUS "Two's complement arithmetic: DETECTED")
    else()
        set(ARCH_TWOS_COMPLEMENT OFF)
        message(STATUS "Two's complement arithmetic: NOT DETECTED (rare platform)")
    endif()
else()
    # Assume two's complement on all modern platforms
    set(ARCH_TWOS_COMPLEMENT ON)
    message(STATUS "Two's complement arithmetic: ASSUMED (modern platform)")
endif()

if(ARCH_TWOS_COMPLEMENT)
    list(APPEND BIT_OPS_COMPILE_DEFINITIONS ARCH_TWOS_COMPLEMENT)
endif()

# Test for signed right shift behavior (arithmetic vs logical)
if(RUN_COMPILER_TESTS)
    try_run(SIGNED_SHIFTS_TEST_RUN SIGNED_SHIFTS_TEST_COMPILE
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/test_signed_shifts.c
        RUN_OUTPUT_VARIABLE SIGNED_SHIFTS_OUTPUT
    )
    if(SIGNED_SHIFTS_TEST_COMPILE AND SIGNED_SHIFTS_TEST_RUN EQUAL 0)
        set(SIGNED_SHIFTS ON)
        message(STATUS "Signed right shifts (arithmetic): DETECTED")
    else()
        set(SIGNED_SHIFTS OFF)
        message(STATUS "Signed right shifts (arithmetic): NOT DETECTED")
    endif()
else()
    # Assume arithmetic right shifts on all modern platforms with two's complement
    if(ARCH_TWOS_COMPLEMENT)
        set(SIGNED_SHIFTS ON)
        message(STATUS "Signed right shifts (arithmetic): ASSUMED (two's complement platform)")
    else()
        set(SIGNED_SHIFTS OFF)
        message(STATUS "Signed right shifts (arithmetic): DISABLED (non-two's complement)")
    endif()
endif()

if(SIGNED_SHIFTS)
    list(APPEND BIT_OPS_COMPILE_DEFINITIONS SIGNED_SHIFTS)
endif()

# Clang format and analysis support
if(CMAKE_C_COMPILER_ID STREQUAL "Clang" AND ENABLE_CLANG_FORMAT)
    find_program(CLANG_FORMAT_EXECUTABLE clang-format)
    if(CLANG_FORMAT_EXECUTABLE)
        # Get all source files
        file(GLOB_RECURSE ALL_SOURCE_FILES
            ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c
            ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
            ${CMAKE_CURRENT_SOURCE_DIR}/test/*.c
            ${CMAKE_CURRENT_SOURCE_DIR}/test/*.h
        )

        # Create format target
        add_custom_target(format
            COMMAND ${CLANG_FORMAT_EXECUTABLE} -i ${ALL_SOURCE_FILES}
            COMMENT "Running clang-format on source files"
        )

        # Create format-check target
        add_custom_target(format-check
            COMMAND ${CLANG_FORMAT_EXECUTABLE} --dry-run --Werror ${ALL_SOURCE_FILES}
            COMMENT "Checking code formatting with clang-format"
        )

        message(STATUS "Clang format targets: ENABLED (run 'make format' or 'make format-check')")
    else()
        message(WARNING "clang-format not found, formatting targets disabled")
    endif()
endif()

# Static analyzer support
if(CMAKE_C_COMPILER_ID STREQUAL "Clang" AND ENABLE_CLANG_ANALYSIS)
    find_program(CLANG_TIDY_EXECUTABLE
        NAMES clang-tidy clang-tidy-14
        DOC "Path to clang-tidy executable"
    )
    if(CLANG_TIDY_EXECUTABLE)
        set(CMAKE_C_CLANG_TIDY ${CLANG_TIDY_EXECUTABLE};
            --header-filter=.*
        )
        message(STATUS "Clang-tidy static analysis: ENABLED")
    else()
        message(WARNING "clang-tidy not found, static analysis disabled")
    endif()

    find_program(SCAN_BUILD_EXECUTABLE
        NAMES scan-build scan-build-14
        DOC "Path to scan-build executable"
    )
    if(SCAN_BUILD_EXECUTABLE)
        add_custom_target(static-analysis
            COMMAND ${SCAN_BUILD_EXECUTABLE} --use-analyzer=${CMAKE_C_COMPILER} make
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Running Clang Static Analyzer"
        )
        message(STATUS "Scan-build static analysis: ENABLED (run 'make static-analysis')")
    else()
        message(WARNING "scan-build not found, static analysis target disabled")
    endif()
endif()

# Create shared library if requested
if(BUILD_SHARED_LIBS)
    add_library(bit_ops_shared SHARED ${BIT_OPS_SOURCES})

    # Set target properties
    set_target_properties(bit_ops_shared PROPERTIES
        OUTPUT_NAME bit_ops
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR}
        PUBLIC_HEADER "include/bit_ops_api.h"
    )

    # Set include directories
    target_include_directories(bit_ops_shared
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src
    )

    # Set compile options
    target_compile_options(bit_ops_shared PRIVATE ${BIT_OPS_COMPILE_FLAGS})

    # Add compile definitions
    target_compile_definitions(bit_ops_shared PUBLIC ${BIT_OPS_COMPILE_DEFINITIONS})

    # Export symbols on Windows
    if(WIN32)
        set_target_properties(bit_ops_shared PROPERTIES
            WINDOWS_EXPORT_ALL_SYMBOLS ON
        )
    endif()
endif()

# Create static library if requested
if(BUILD_STATIC_LIBS)
    add_library(bit_ops_static STATIC ${BIT_OPS_SOURCES})

    # Set target properties
    set_target_properties(bit_ops_static PROPERTIES
        OUTPUT_NAME bit_ops
        PUBLIC_HEADER "include/bit_ops_api.h"
    )

    # Set include directories
    target_include_directories(bit_ops_static
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src
    )

    # Set compile options
    target_compile_options(bit_ops_static PRIVATE ${BIT_OPS_COMPILE_FLAGS})

    # Add compile definitions
    target_compile_definitions(bit_ops_static PUBLIC ${BIT_OPS_COMPILE_DEFINITIONS})
endif()

# Create alias targets for easier linking
if(BUILD_SHARED_LIBS)
    add_library(bit_ops::shared ALIAS bit_ops_shared)
endif()

if(BUILD_STATIC_LIBS)
    add_library(bit_ops::static ALIAS bit_ops_static)
endif()

# Default target (prefer shared if both are built)
if(BUILD_SHARED_LIBS)
    add_library(bit_ops::bit_ops ALIAS bit_ops_shared)
elseif(BUILD_STATIC_LIBS)
    add_library(bit_ops::bit_ops ALIAS bit_ops_static)
endif()

# Build tests if requested
if(BUILD_TESTS)
    add_subdirectory(test)
endif()

# Installation configuration
include(GNUInstallDirs)

# Install libraries
if(BUILD_SHARED_LIBS)
    install(TARGETS bit_ops_shared
        EXPORT bit_ops-targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/bit_ops
    )
endif()

if(BUILD_STATIC_LIBS)
    install(TARGETS bit_ops_static
        EXPORT bit_ops-targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/bit_ops
    )
endif()

# Install header files
install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/bit_ops
    FILES_MATCHING PATTERN "*.h"
)

# Install export targets
install(EXPORT bit_ops-targets
    FILE bit_ops-targets.cmake
    NAMESPACE bit_ops::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/bit_ops
)

# Create and install config file
include(CMakePackageConfigHelpers)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/bit_ops-config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/bit_ops-config.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/bit_ops
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/bit_ops-config-version.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/bit_ops-config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/bit_ops-config-version.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/bit_ops
)

# Print build configuration
message(STATUS "bit_ops library configuration:")
message(STATUS "  Version: ${PROJECT_VERSION}")
message(STATUS "  Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  C Compiler: ${CMAKE_C_COMPILER_ID} ${CMAKE_C_COMPILER_VERSION}")
message(STATUS "  Build shared libraries: ${BUILD_SHARED_LIBS}")
message(STATUS "  Build static libraries: ${BUILD_STATIC_LIBS}")
message(STATUS "  Build tests: ${BUILD_TESTS}")
if(LINUX_BUILD)
    message(STATUS "  Platform: Linux")
elseif(WINDOWS_BUILD)
    message(STATUS "  Platform: Windows")
else()
    message(STATUS "  Platform: ${CMAKE_SYSTEM_NAME}")
endif()
if(NATIVE_64BIT_UNSUPPORTED)
    message(STATUS "  Native 64-bit types: UNSUPPORTED (using struct emulation)")
else()
    message(STATUS "  Native 64-bit types: SUPPORTED")
endif()
message(STATUS "  Compiler definitions: ${BIT_OPS_COMPILE_DEFINITIONS}")