//
// Created by yj374 on 4/22/18.
//

#include <matrix/sparse_matrix.h>
#include <vector>
#include <iostream>

void dirichlet() {
	std::vector<double> vals = {-2, 1, 1, -2, 1, 1, -2, 1, 1, -2};
	std::vector<double> row_ptr = {0, 2, 5, 8, 10};
	std::vector<double> col_idx = {0, 1, 0, 1, 2, 1, 2, 3, 2, 3};
	sparse_matrix<double> s = sparse_matrix<double>::generate_test_matrix(vals, row_ptr, col_idx);
	s.report();
	
	std::vector<double> b = {0.04, 0, 0, 0};
	auto x = s.solve(b, 1e-8, 500, sparse_matrix<double>::jacobi);
	for(auto ix: x) {
		std::cout << ix << " ";
	}
	std::cout << std::endl;
}

void neumann() {
	std::vector<double> vals = {-2, 1, 1, -2, 1, 1, -2, 1, 1, -1};
	std::vector<double> row_ptr = {0, 2, 5, 8, 10};
	std::vector<double> col_idx = {0, 1, 0, 1, 2, 1, 2, 3, 2, 3};
	sparse_matrix<double> s = sparse_matrix<double>::generate_test_matrix(vals, row_ptr, col_idx);
	s.report();
	
	std::vector<double> b = {0.04, 0, 0, 0};
	auto x = s.solve(b, 1e-8, 500, sparse_matrix<double>::jacobi);
	for(auto ix: x) {
		std::cout << ix << " ";
	}
	std::cout << std::endl;
}


int main(int argc, char** argv) {
	dirichlet();
	neumann();
	return 0;
}

