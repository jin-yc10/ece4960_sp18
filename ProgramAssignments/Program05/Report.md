#### Structure

The structure of Program Assignment 05 is as follow:
1. Created the 'sparse' directory to contain the algorithms 
that works for sparse-matrix.
2. Created the 'ProgramAssignments/Program05' to contain the report and results.

#### Algorithms
In this assignments, I implemented two decomposition algorithms, QR and SVD.

sparse/matrix/sparse_matrix.h & cpp was borrowed from previous homework and assignments.

so start from this sparse_matrix implementation, I built QR and SVD decomposition implementation.
Since QR is usually applied to dense matrix, I used cv::Mat to handle matrix data structure.
In SVD solve module, I used the sparse matrix.

1. Sparse Matrix:
I developed several related apis to maintain the sparse-matrix. 
To fulfill the requirements of calculating the decomposition.
I exposed the get_row_idx, get_element_by_id and get_col_by_id 
to provide better access to the internal data structure.

2. QR Decomposition:
In general, we have three different methods of solving QR decomposition.
Gram-Schmidt, HouseHolder and Givens Rotation. They are both in sparse/qr/sparse_qr.h.
To test the correctness of the decomposition, I compute Q*R-A. 
The difference of this formula should be very close to zero. The input matrix was generate by a random function cv::randu.
So the input matrix A should be filled with uniform-distribution numbers.

3. SVD Decomposition:
I chose the Jacobi iteration algorithm to solve the SVD decomposition. 
Since it's not a trivial job to check the correctness. 
I compare my result to the output of exist software solvers: numpy.linalg.svd and svd in opencv.
The basic idea is to sweep the whole non-diagnal elements, then we use jacobi-rotation to eliminate the non-zero elements.
Usually the algorithm should converge in some iterations. 
This approach is somehow slow but it has better numerical stability.
Currently people proposed several updates to this method.


#### Test and QA
1. Test:
This assignment included the 'googletest' framework ( 3rd/googletest ) 
to create better tests. 
It's a submodule so you should use ' git submodule init && git submodule update' 
in order to fetch this project.  
All the tests are in sparse/test folder named sparse_test_[test-target].cpp. 
Each module has its own corresponding test .cpp file.
The result of the test-suite is also included in this folder.

2. QA:
  Since these algorithms are already developed and maintain by 
  several different organizatinos and companies. I decided to work in this way.
  First I survey the usage of several mature packages including OpenCV, Eigen3 and Numpy.
  They provided similar interface so I design my interface accordingly. 
  By running these software and packages, I got both some basic 
  knowledge and the answer to specific input matrix.
  So now my target became: create an algorithm that can generate the same output as these packages.
  
3. Usage
  Since currently my algorithm is quite simple and straghtforward, 
  I think user can learn from the test files. 
  These tests can give users basic knowledge on how to use these libraries.

