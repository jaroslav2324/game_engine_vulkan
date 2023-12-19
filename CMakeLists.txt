cmake_minimum_required(VERSION 3.22)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_C_COMPILER "clang-15")
set(CMAKE_CXX_COMPILER "clang++-15")

set (CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

project(game_engine)


if (UNIX)

endif(UNIX)

set (LIB_ENGINE_SOURCE_FILES ${CMAKE_SOURCE_DIR}/engine/src/test.cpp ${CMAKE_SOURCE_DIR}/engine/src/engine.cpp)
set (LIB_ENGINE_INCLUDE_FOLDER ${CMAKE_SOURCE_DIR}/engine/include)

add_library(engine SHARED ${LIB_ENGINE_SOURCE_FILES})
target_include_directories(engine PUBLIC LIB_ENGINE_INCLUDE_FOLDER)
target_link_libraries(engine glfw vulkan dl pthread X11 Xxf86vm Xrandr Xi)
target_compile_definitions(engine PRIVATE ENGINE_EXPORTS)



set(TEST_SOURCE_FILES ${CMAKE_SOURCE_DIR}/test/src/main.cpp)

add_executable(testengine ${TEST_SOURCE_FILES})
target_include_directories(testengine PRIVATE ${LIB_ENGINE_INCLUDE_FOLDER})
target_link_libraries(testengine engine)

# test link 
# target_include_directories(testengine PRIVATE )
# target_link_libraries(testengine glfw vulkan dl pthread X11 Xxf86vm Xrandr Xi)
