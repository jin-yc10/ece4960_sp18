cmake_minimum_required(VERSION 3.5)
project(ece4960_sp18)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(matrix)
add_subdirectory(numeric)

add_subdirectory(HackerPractices)
add_subdirectory(Hw)

add_subdirectory(ProgramAssignments/Program01)

add_executable(ece4960_sp18 main.cpp)