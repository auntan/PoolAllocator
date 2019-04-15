cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

project(tests)

# https://cmake.org/cmake/help/latest/prop_tgt/CXX_STANDARD.html
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(tests tests.cpp)

target_include_directories(tests PUBLIC ../include/)
