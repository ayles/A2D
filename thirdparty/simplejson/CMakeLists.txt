cmake_minimum_required(VERSION 3.2)

project(SimpleJSON)

add_library(simplejson STATIC JSONValue.cpp JSON.cpp)
target_include_directories(simplejson PUBLIC include)