cmake_minimum_required(VERSION 3.7)
project(ece4960_sp18)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(Note02)
add_subdirectory(Note03)
add_executable(ece4960_sp18 main.cpp)