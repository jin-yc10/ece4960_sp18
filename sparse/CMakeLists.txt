find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS} ${DLIB_INCLUDE})

add_executable(sparse_test 
  test/sparse_test_main.cpp 
  test/sparse_test_matrix.cpp
  test/sparse_test_svd.cpp
  test/sparse_test_qr.cpp
)
target_link_libraries(sparse_test gtest gtest_main gmock gmock_main ${OpenCV_LIBS} )