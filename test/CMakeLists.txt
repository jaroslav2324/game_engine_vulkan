cmake_minimum_required(VERSION 3.22)

project(testengine)

set (LIB_INCLUDE_FOLDER ${CMAKE_SOURCE_DIR}/engine/include)
message("Include folder: "${LIB_INCLUDE_FOLDER})
find_library(ENGINE_LIB "engine" PATHS ${CMAKE_SOURCE_DIR}/build/engine)
message("Lib: " ${ENGINE_LIB})

if (WIN32)
    set(SDL3_INCLUDE_DIRS "C:\\Libs\\SDL\\include")
    set(SDL3_LIBRARIES "C:/Libs/SDL3/SDL3.lib")
endif(WIN32)

if (UNIX)
    find_package(SDL3 REQUIRED)
endif(UNIX)


set(TEST_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

add_executable(testengine ${TEST_SOURCE_FILES} )
target_include_directories(testengine PRIVATE ${LIB_INCLUDE_FOLDER} PRIVATE ${SDL3_INCLUDE_DIRS})
target_link_libraries(testengine ${ENGINE_LIB} ${SDL3_LIBRARIES})