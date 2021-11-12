cmake_minimum_required(VERSION 3.2)

project(efgl VERSION 0.0.1 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(VENDOR ${PROJECT_SOURCE_DIR}/vendor)

set(SOURCE_DIR ${PROJECT_SOURCE_DIR}/efgl/src)
file(GLOB_RECURSE SOURCES ${SOURCE_DIR}/**/*.cpp)
file(GLOB TOPLEVEL_SOURCES ${SOURCE_DIR}/*.cpp)
set(SOURCES ${SOURCES} ${TOPLEVEL_SOURCES})
add_executable(efgl ${SOURCES})

target_compile_definitions(efgl PRIVATE DEBUG)

set(INCLUDES ${SOURCE_DIR}
  ${SOURCE_DIR}/application
  ${SOURCE_DIR}/geometry
  ${SOURCE_DIR}/geometry/prim
  ${SOURCE_DIR}/material
  ${SOURCE_DIR}/platform/OpenGL
  ${SOURCE_DIR}/platform/OpenGL/fbo
  ${SOURCE_DIR}/render
  ${SOURCE_DIR}/scene
  ${SOURCE_DIR}/texture
  ${SOURCE_DIR}/util)
set(INCLUDES ${INCLUDES} 
  ${VENDOR}/assimp/include
  ${VENDOR}/Glad/include
  ${VENDOR}/GLFW/include
  ${VENDOR}/glm
  ${VENDOR}/imgui
  ${VENDOR}/stb
  ${VENDOR}/tracy)
target_include_directories(efgl PRIVATE ${INCLUDES})

add_subdirectory(vendor/assimp)
target_link_libraries(efgl PRIVATE assimp)

add_subdirectory(vendor/Glad)
target_link_libraries(efgl PRIVATE glad)

add_subdirectory(vendor/GLFW)
target_link_libraries(efgl PRIVATE glfw)

add_subdirectory(vendor/glm)
target_link_libraries(efgl PRIVATE glm)

add_subdirectory(vendor/imgui)
target_link_libraries(efgl PRIVATE imgui)

target_precompile_headers(efgl 
  PRIVATE ${SOURCE_DIR}/efpch.h)

message(${SOURCES})
