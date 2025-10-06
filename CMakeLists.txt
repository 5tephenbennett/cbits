cmake_minimum_required(VERSION 3.10)

# Top-level project
project(cbits VERSION 0.2 LANGUAGES C CXX)

# Generate compile_commands.json for IDE support
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set default build type to Release if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Global options that apply to all subprojects
option(BUILD_SHARED_LIBS "Build shared libraries" ON)
option(BUILD_STATIC_LIBS "Build static libraries" ON)
option(BUILD_TESTS "Build test programs" ON)
option(RUN_COMPILER_TESTS "Run compiler capability tests" ON)
option(ENABLE_CLANG_ANALYSIS "Enable Clang static analysis and sanitizers" ON)
option(ENABLE_CLANG_FORMAT "Enable clang-format style checking" ON)

# Add subprojects
add_subdirectory(bit_ops)

# Print top-level configuration
message(STATUS "cbits project configuration:")
message(STATUS "  Version: ${PROJECT_VERSION}")
message(STATUS "  Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  C Compiler: ${CMAKE_C_COMPILER_ID} ${CMAKE_C_COMPILER_VERSION}")
message(STATUS "  Build shared libraries: ${BUILD_SHARED_LIBS}")
message(STATUS "  Build static libraries: ${BUILD_STATIC_LIBS}")
message(STATUS "  Build tests: ${BUILD_TESTS}")
message(STATUS "  Run compiler tests: ${RUN_COMPILER_TESTS}")
message(STATUS "  Enable Clang analysis: ${ENABLE_CLANG_ANALYSIS}")
message(STATUS "  Enable Clang format: ${ENABLE_CLANG_FORMAT}")