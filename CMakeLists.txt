cmake_minimum_required(VERSION 3.10)

project(mos6502 VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# -------------------------------
# Fetch GoogleTest
# -------------------------------
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# -------------------------------
# Build the core emulator library
# -------------------------------
add_library(mos6502_core
  ${PROJECT_SOURCE_DIR}/src/CPU.cpp
)

target_include_directories(mos6502_core
  PUBLIC ${PROJECT_SOURCE_DIR}/include
)

# -------------------------------
# Main emulator binary (optional)
# -------------------------------
add_executable(mos6502
  ${PROJECT_SOURCE_DIR}/src/main.cpp
)

target_link_libraries(mos6502 PRIVATE mos6502_core)

# -------------------------------
# Testing setup
# -------------------------------
enable_testing()

add_executable(mos6502_tests
  ${PROJECT_SOURCE_DIR}/test/test.cpp
)

target_link_libraries(mos6502_tests
  GTest::gtest_main
  mos6502_core
)

target_include_directories(mos6502_tests
  PUBLIC ${PROJECT_SOURCE_DIR}/include
)

include(GoogleTest)
gtest_discover_tests(mos6502_tests)
