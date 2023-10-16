cmake_minimum_required(VERSION 3.22)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")

set (CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

project(game_engine)

# message(STATUS "Building all targets...")
# message("Current source dir: " ${CMAKE_CURRENT_SOURCE_DIR})
# message("Current binary dir: " ${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(engine)
add_subdirectory(test)


