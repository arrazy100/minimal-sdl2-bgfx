cmake_minimum_required(VERSION 3.10)
project(sdl_bgfx_minimal)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(SDL2 REQUIRED)

add_subdirectory(bgfx.cmake)
include_directories(bgfx/include)

add_executable(sdl_bgfx_minimal main.cpp)
target_link_libraries(sdl_bgfx_minimal bx bgfx SDL2::Main)