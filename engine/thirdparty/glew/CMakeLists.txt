cmake_minimum_required(VERSION 3.10)

project(GLEW)

add_library(glew STATIC src/glew.c)
target_include_directories(glew PUBLIC "include")