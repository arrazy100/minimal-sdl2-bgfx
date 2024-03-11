cmake_minimum_required(VERSION 3.10)
project(sdl_bgfx_minimal)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(SDL2 REQUIRED)

add_subdirectory(bgfx.cmake)
add_subdirectory(shader)

add_executable(${PROJECT_NAME} main.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC include/shaders)
target_link_libraries(${PROJECT_NAME} bx bgfx SDL2::Main)