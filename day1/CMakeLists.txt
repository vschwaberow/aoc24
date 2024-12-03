cmake_minimum_required(VERSION 3.26)

project(HistorianHysteria VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(HistorianHysteria day1.cc)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(HistorianHysteria PRIVATE -Wall -Wextra -Wpedantic -O3)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_options(HistorianHysteria PRIVATE /W4 /O2)
endif()
