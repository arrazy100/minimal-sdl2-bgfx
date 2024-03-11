cmake_minimum_required(VERSION 3.10)
project(bgfx_shaders)

MACRO(SUBDIRLIST result curdir)
    FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
    SET(dirlist "")
    FOREACH(child ${children})
        IF(IS_DIRECTORY ${curdir}/${child})
            LIST(APPEND dirlist ${child})
        ENDIF()
    ENDFOREACH()
    SET(${result} ${dirlist})
ENDMACRO()

set(BGFX_DIR ${CMAKE_SOURCE_DIR}/bgfx.cmake/bgfx)
set(SHADERS_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/shaders)

add_library(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE main.cpp)

include(${CMAKE_SOURCE_DIR}/bgfx.cmake/cmake/bgfxToolUtils.cmake)
subdirlist(SHADER_DIRS "${SHADERS_ROOT}")
foreach(DIR_NAME ${SHADER_DIRS})
  bgfx_compile_shader_to_header(
    TYPE VERTEX
    SHADERS ${SHADERS_ROOT}/${DIR_NAME}/vs_${DIR_NAME}.sc
    VARYING_DEF ${SHADERS_ROOT}/${DIR_NAME}/varying.def.sc
    OUTPUT_DIR ${CMAKE_SOURCE_DIR}/include/shaders/${DIR_NAME}
    INCLUDE_DIRS ${BGFX_DIR}/src
  )

  bgfx_compile_shader_to_header(
    TYPE FRAGMENT
    SHADERS ${SHADERS_ROOT}/${DIR_NAME}/fs_${DIR_NAME}.sc
    VARYING_DEF ${SHADERS_ROOT}/${DIR_NAME}/varying.def.sc
    OUTPUT_DIR ${CMAKE_SOURCE_DIR}/include/shaders/${DIR_NAME}
    INCLUDE_DIRS ${BGFX_DIR}/src
  )

  target_sources(${PROJECT_NAME} PRIVATE ${SHADERS_ROOT}/${DIR_NAME}/vs_${DIR_NAME}.sc)
  target_sources(${PROJECT_NAME} PRIVATE ${SHADERS_ROOT}/${DIR_NAME}/fs_${DIR_NAME}.sc)
endforeach()

# target_include_directories(bgfx_shaders PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/generated/shaders)