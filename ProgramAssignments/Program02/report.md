## Program Assignment 02 Report

0. The Program Assignment itself was in 'program02.cpp'. 
  I used 'validate' function to run a specific case.
  The iterative solver was implemented from raw data rather than decomposited the matrix as LU matrix.
  In this way, the code was much shorter and faster since there is no need to allocate extra memory.
  According to the result, both the jacobi and gauss-seidel method need 6-8 steps and took about 0.05s.
  Both of them could converge and got pretty good result. SOR method were not so easy. 
  The argument w was hard to pick.
  And it was pretty obvious that the sparse matrix could come up with the result way much faster than dense matrix.
  According to the nested for loop, we know that we can save many mulplications from compressed representation.
  Detailed output can be found in 'output_program02.txt'.
1. Test Suite and Verification: 
 The test for matrix operations are in 'test_matrix.h'. 
 All the basic operations are included as 'test_[method name]'.
 
2. Matrix Operators
 Operators are implemented as slides indicated. 
 To verify them, I add a 'norm_diff' function between sparse and dense matrix.
 If sp is a sparse matrix, de is a dense matrix, sp.norm_diff(de) will give us the difference of these two matrix.
 
3. Time and memory usage
 For further usage, I put these two parts into utils.
 Timer part is in tick-tock manner.
```
timer_util stopwatch;
stopwatch.tick();
[some code]
stopwatch.tock("[some description]");
```
  Memory usage was request from system library. 
  I reference from http://stackoverflow.com/questions/63166/how-to-determine-cpu-and-memory-consumption-from-inside-a-process 
  and https://github.com/DigitalInBlue/Npas4.
  
4. Design as library functions: Both sparse and dense matrix were implemented as a class.
So the solvers and other operators are implemented as class member functions. 
Which can be easily used in the future.
  
5. In-code design:
   Basically the document in code is in doxygen format.
   Since the code is quite straight-forward, so the document was not as detailed as other libraries like sparse-suite.
   