cmake_minimum_required(VERSION 3.10)

project(AOC24)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

file(GLOB DAY_DIRS "day*")

foreach(DIR ${DAY_DIRS})
    if(IS_DIRECTORY ${DIR})
        add_subdirectory(${DIR})
    endif()
endforeach()