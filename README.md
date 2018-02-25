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
|- Hw
|   |- Hw04, PartI.cpp, PartII.cpp
|- matrix: matrix library
|   |- dense and sparse matrix implementation
|- numeric: numeric library
|- ProgramAssignments
|- utils: utility functions
|   |- mem_usage: measure the memory usages, cross-platform, ref: https://github.com/DigitalInBlue/Npas4
|   |- output: double and float to hex string
|   |- timer: for stopwatch, use std::chrono::steady_clock ( supported in c++11 )
```