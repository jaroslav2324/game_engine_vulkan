cmake_minimum_required(VERSION 3.22)

project(engine)

if (WIN32)
    set(SDL3_INCLUDE_DIRS "C:/Libs/SDL/include")

    find_library(SDL3_LIBRARIES NAMES SDL3 SDL3main PATHS C:/Libs/SDL3/)
    message(${SDL3_LIBRARIES})
endif(WIN32)

if (UNIX)
    find_package(SDL3 REQUIRED)
endif(UNIX)

# message(${CMAKE_SOURCE_DIR})
set (LIB_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/test.cpp)

add_library(engine SHARED ${LIB_SOURCE_FILES})
target_include_directories(engine PUBLIC ${SDL3_INCLUDE_DIRS})
target_link_libraries(engine PUBLIC ${SDL3_LIBRARIES})
target_compile_definitions(engine PRIVATE ENGINE_EXPORTS)

