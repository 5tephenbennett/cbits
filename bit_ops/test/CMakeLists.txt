# Test programs for bit_ops library
# Provides interactive test application and Google Test suite

# ============================================================================
# Interactive Test Application
# ============================================================================

# Interactive test source files
set(INTERACTIVE_SOURCES
    interactive/main.c
    interactive/bit_ops_cmds.c
    interactive/bit8_cmds.c
    interactive/bit16_cmds.c
    interactive/bit32_cmds.c
    interactive/bit64_cmds.c
)

# Create interactive test executable
add_executable(bit_ops_interactive ${INTERACTIVE_SOURCES})

# Link with bit_ops library
if(TARGET bit_ops_shared)
    target_link_libraries(bit_ops_interactive PRIVATE bit_ops_shared)
else()
    target_link_libraries(bit_ops_interactive PRIVATE bit_ops_static)
endif()

target_include_directories(bit_ops_interactive PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

target_compile_definitions(bit_ops_interactive PRIVATE ${BIT_OPS_COMPILE_DEFINITIONS})
target_compile_options(bit_ops_interactive PRIVATE ${BIT_OPS_COMPILE_FLAGS})
target_compile_features(bit_ops_interactive PRIVATE c_std_99)

# Disable optimization for tests to prevent memory issues with large functions
target_compile_options(bit_ops_interactive PRIVATE -O0)

# Always enable enhanced sanitizers for test/interactive programs
if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    target_compile_options(bit_ops_interactive PRIVATE ${GCC_SANITIZER_FLAGS})
    target_link_options(bit_ops_interactive PRIVATE ${GCC_SANITIZER_LINK_FLAGS})
    message(STATUS "GCC enhanced sanitizers for interactive tests: ENABLED")

    # Always enable coverage for tests
    if(ENABLE_COVERAGE)
        target_compile_options(bit_ops_interactive PRIVATE ${GCC_COVERAGE_FLAGS})
        target_link_options(bit_ops_interactive PRIVATE ${GCC_COVERAGE_LINK_FLAGS})
        message(STATUS "GCC coverage for interactive tests: ENABLED")
    endif()
elseif(CMAKE_C_COMPILER_ID STREQUAL "Clang" OR CMAKE_C_COMPILER_ID STREQUAL "AppleClang")
    target_compile_options(bit_ops_interactive PRIVATE ${CLANG_SANITIZER_FLAGS})
    target_link_options(bit_ops_interactive PRIVATE ${CLANG_SANITIZER_LINK_FLAGS})
    message(STATUS "Clang enhanced sanitizers for interactive tests: ENABLED")
endif()

# ============================================================================
# Google Test
# ============================================================================

# Enable C++ for Google Test
enable_language(CXX)

# Find or fetch Google Test
find_package(GTest QUIET)

if(NOT GTest_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
endif()

# Google Test source files
set(GTEST_SOURCES
    gtest/gtest_main.cpp
    gtest/bit8_gtest.cpp
    gtest/bit16_gtest.cpp
    gtest/bit32_gtest.cpp
    gtest/bit64_gtest.cpp
    gtest/bit_macros_gtest.cpp
)

# Create Google Test executable
add_executable(bit_ops_gtest ${GTEST_SOURCES})

# Link with libraries
if(TARGET bit_ops_shared)
    target_link_libraries(bit_ops_gtest PRIVATE bit_ops_shared)
else()
    target_link_libraries(bit_ops_gtest PRIVATE bit_ops_static)
endif()

if(GTest_FOUND)
    target_link_libraries(bit_ops_gtest PRIVATE GTest::gtest GTest::gtest_main)
else()
    target_link_libraries(bit_ops_gtest PRIVATE gtest gtest_main)
endif()

target_include_directories(bit_ops_gtest PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

target_compile_definitions(bit_ops_gtest PRIVATE ${BIT_OPS_COMPILE_DEFINITIONS})
target_compile_options(bit_ops_gtest PRIVATE ${BIT_OPS_COMPILE_FLAGS})
target_compile_features(bit_ops_gtest PRIVATE cxx_std_11)

# Disable optimization for tests to prevent memory issues with large test functions
# Tests don't need optimization and large functions can overwhelm the compiler
target_compile_options(bit_ops_gtest PRIVATE -O0)

# Always enable enhanced sanitizers for test/interactive programs
if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    target_compile_options(bit_ops_gtest PRIVATE ${GCC_SANITIZER_FLAGS})
    target_link_options(bit_ops_gtest PRIVATE ${GCC_SANITIZER_LINK_FLAGS})
    message(STATUS "GCC enhanced sanitizers for Google Test: ENABLED")

    # Always enable coverage for tests
    if(ENABLE_COVERAGE)
        target_compile_options(bit_ops_gtest PRIVATE ${GCC_COVERAGE_FLAGS})
        target_link_options(bit_ops_gtest PRIVATE ${GCC_COVERAGE_LINK_FLAGS})
        message(STATUS "GCC coverage for Google Test: ENABLED")
    endif()
elseif(CMAKE_C_COMPILER_ID STREQUAL "Clang" OR CMAKE_C_COMPILER_ID STREQUAL "AppleClang")
    target_compile_options(bit_ops_gtest PRIVATE ${CLANG_SANITIZER_FLAGS})
    target_link_options(bit_ops_gtest PRIVATE ${CLANG_SANITIZER_LINK_FLAGS})
    message(STATUS "Clang enhanced sanitizers for Google Test: ENABLED")
endif()