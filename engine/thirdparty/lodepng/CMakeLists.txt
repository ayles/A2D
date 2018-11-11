cmake_minimum_required(VERSION 3.10)

project(LodePNG)

add_library(lodepng STATIC lodepng.cpp)
target_include_directories(lodepng PUBLIC "include")