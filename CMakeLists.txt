cmake_minimum_required(VERSION 3.22)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")

project(game_engine)


set (LIB_INCLUDE_FOLDER "../engine/include")
set (LIB_SOURCE_FILES "../engine/src/test.cpp")

set (CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

add_library(enginelib SHARED ${LIB_SOURCE_FILES})



find_library(ENGINE_LIB "enginelib")

set(TEST_SOURCE_FILES "../test/src/main.cpp")

include_directories(${LIB_INCLUDE_FOLDER})

add_executable(testengine ${TEST_SOURCE_FILES})
target_link_libraries(testengine ${ENGINE_LIB})

