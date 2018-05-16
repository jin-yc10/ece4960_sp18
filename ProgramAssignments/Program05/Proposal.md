This matrix solver includes both direct and iterative solver.
And the feature should be:
1. Matrix condition checking
2. Can deal with non-square matrix
3. Provide some decomposition algorithms
4. svd decomposition

The test should include:
1. Scale test, how the running time of different algorithms change according to the scale of the problem.
2. Accuracy test, if the matrix is ill-conditioning, how stable are the results.
3. Real world application demonstration. I would like to show the result of the solver by 'solvePnP' problem. It's to solve the translation parameters T such that x = TX.

