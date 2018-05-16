cmake_minimum_required(VERSION 3.5)
project(ece4960_sp18)

set(CMAKE_CXX_STANDARD 11)

include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/3rd/googletest/include)

if( WIN32 )
    set(DLIB_LIB C:/Dev/dlib_install/lib/dlib19.10.99_debug_64bit_msvc1900.lib)
    set(DLIB_INCLUDE C:/Dev/dlib_install/include)
elseif(APPLE)
    add_definitions(-DGTEST_USE_OWN_TR1_TUPLE)
    add_definitions(-D__GLIBCXX__)
    set(DLIB_LIB /Users/jin-yc10/dev/dlib/build/lib/libdlib.a)
    set(DLIB_INCLUDE /Users/jin-yc10/dev/dlib/build/include /opt/intel/compilers_and_libraries_2018.1.126/mac/mkl/include)
else()
endif()

#add_subdirectory(matrix)
#add_subdirectory(numeric)

#add_subdirectory(HackerPractices)
#add_subdirectory(Hw)

#add_subdirectory(ProgramAssignments/Program01)
#add_subdirectory(ProgramAssignments/Program02)
#add_subdirectory(Playground)
#add_subdirectory(ProgramAssignments/Program03)
#add_subdirectory(ProgramAssignments/Program04)

add_subdirectory(3rd/googletest)
add_subdirectory(sparse)
add_subdirectory(Final)
add_subdirectory(ProgramAssignments/Program05)
#add_executable(ece4960_sp18 main.cpp)