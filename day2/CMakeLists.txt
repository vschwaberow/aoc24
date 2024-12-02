cmake_minimum_required(VERSION 3.26)

project(aoc2 VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Erstelle beide Executables
add_executable(part1 part1.cc)
add_executable(part2 part2.cc)

# Compiler-Flags für beide Executables
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(part1 PRIVATE -Wall -Wextra -Wpedantic -O3)
    target_compile_options(part2 PRIVATE -Wall -Wextra -Wpedantic -O3)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_options(part1 PRIVATE /W4 /O2)
    target_compile_options(part2 PRIVATE /W4 /O2)
endif()