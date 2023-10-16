cmake_minimum_required(VERSION 3.22)

project(engine)

if (WIN32)
    set(SDL3_INCLUDE_DIRS "C:/Libs/SDL/include")
    set(SDL3_LIBRARIES "C:/Libs/SDL3/SDL3.lib")
endif(WIN32)

if (UNIX)
    find_package(SDL3 REQUIRED)
endif(UNIX)

# message(${CMAKE_SOURCE_DIR})
set (LIB_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/test.cpp)

add_library(engine SHARED ${LIB_SOURCE_FILES})
target_include_directories(engine PUBLIC ${SDL3_INCLUDE_DIRS})

