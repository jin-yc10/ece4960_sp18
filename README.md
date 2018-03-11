# ece4960_sp18
For ECE4960

Project structure:

CMake Version should be greater than 3.5
This project is tested mostly on windows and mac os x

```
Project Root
|- HackerPractices
|   |- Note02
|   |- Note03
|   |- Note04
|   |- Note05: test_bisection.cpp, test_newton_raphson.cpp 
|- Hw
|   |- Hw04, PartI.cpp, PartII.cpp
|- matrix: matrix library
|   |- dense and sparse matrix implementation
|- nonlinear: nonlinear optimization
|   |- bisection.h and .cpp, bisection search, 1d and 2d
|   |- newton.h and .cpp newton-raphson method
|- numeric: numeric library
|- ProgramAssignments
|- utils: utility functions
|   |- mem_usage: measure the memory usages, cross-platform, ref: https://github.com/DigitalInBlue/Npas4
|   |- output: double and float to hex string
|   |- timer: for stopwatch, use std::chrono::steady_clock ( supported in c++11 )
```