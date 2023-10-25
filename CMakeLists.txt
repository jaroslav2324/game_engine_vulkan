cmake_minimum_required(VERSION 3.22)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")

set (CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

project(game_engine)

if (WIN32)
    set(SDL3_INCLUDE_DIRS "C:/Libs/SDL/include")
    set(SDL3_LIBRARIES "C:/Libs/SDL3/SDL3.lib")
endif(WIN32)

if (UNIX)
    find_package(SDL3 REQUIRED)
endif(UNIX)

set (LIB_SOURCE_FILES ${CMAKE_SOURCE_DIR}/engine/src/test.cpp)
set (LIB_INCLUDE_FOLDER ${CMAKE_SOURCE_DIR}/engine/include)

add_library(engine SHARED ${LIB_SOURCE_FILES})
target_include_directories(engine PUBLIC ${SDL3_INCLUDE_DIRS})
target_link_libraries(engine ${SDL3_LIBRARIES})



set(TEST_SOURCE_FILES ${CMAKE_SOURCE_DIR}/test/src/main.cpp)

add_executable(testengine ${TEST_SOURCE_FILES})
target_include_directories(testengine PRIVATE ${LIB_INCLUDE_FOLDER})
target_link_libraries(testengine engine)


