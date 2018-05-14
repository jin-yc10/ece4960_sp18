set(SRCS ${CMAKE_SOURCE_DIR}/lsq/lsq.cpp ${CMAKE_SOURCE_DIR}/lsq/functor.cpp 
         ${CMAKE_SOURCE_DIR}/nonlinear/find_min.cpp 
)
file(GLOB fs "*.cpp")
get_filename_component(hp_name ${CMAKE_CURRENT_SOURCE_DIR} NAME_WE)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS} C:/Dev/dlib_install/include)
link_directories()
foreach(f ${fs})
    get_filename_component(fn ${f} NAME_WE)
    add_executable(${hp_name}_${fn} ${fn} ${SRCS})
    target_link_libraries(${hp_name}_${fn} ${OpenCV_LIBS} 
    C:/Dev/dlib_install/lib/dlib19.10.99_debug_64bit_msvc1900.lib)
endforeach()
