cmake_minimum_required(VERSION 3.2)
project(glad)

add_library(glad STATIC 
  src/glad.c 
  include/glad/glad.h
  include/KHR/khrplatform.h)

target_include_directories(glad PRIVATE include)
target_link_libraries(glad PRIVATE ${CMAKE_DL_LIBS})
