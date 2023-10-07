cmake_minimum_required(VERSION 3.22)

project(game_engine)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")

# set(CMAKE_COMPILER_C)

add_executable(engine ../src/main.cpp)


