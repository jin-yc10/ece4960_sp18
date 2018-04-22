//
// Created by yj374 on 4/22/18.
//

#include <matrix/sparse_matrix.h>

int main(int argc, char** argv) {
	double h = 1e-6;
	double q = 1.6e-19;
	double eps = 1.03e-12;
	double n_i = 1.5e10;
	double ND = 1e15;
	double NA = 1e17;
	double psi_0 = 1.0;
	double LD = 2.4e10-3;
	
	std::vector<double> vals = {-4, 1, 1,
	                            1, -4, 1, 1,
	                            1, -4, 1,
	                            1, -4, 1, 1,
	                            1, 1, -4, 1, 1,
															1, 1, -4, 1,
															1, -4, 1,
															1, 1, -4, 1,
															1, 1, -4
	};
	std::vector<double> row_ptr = {0, 3, 7, 10, 14, 19, 23, 26, 30, 33};
	std::vector<double> col_idx = {0, 1, 3, //3
	                               0, 1, 2, 4, //7
	                               1, 2, 5, // 10
	                               0, 3, 4, 6, // 14
	                               1, 3, 4, 5, 7, // 19
	                               2, 4, 5, 8, // 23
	                               3, 6, 7, // 26
	                               4, 6, 7, 8, // 30
	                               5, 7, 8 // 33
	};
	sparse_matrix<double> s = sparse_matrix<double>::generate_test_matrix(vals, row_ptr, col_idx);
	s.report();
	
	std::vector<double> b;
	double h2 = h*h;
	b.push_back(h2*ND+2.0*psi_0);
	b.push_back(h2*ND);
	b.push_back(-h2*NA);b.push_back(-h2*ND);
	b.push_back(-h2*NA);b.push_back(-h2*NA);b.push_back(-h2*NA);b.push_back(-h2*NA);b.push_back(-h2*NA);
	auto x = s.solve(b, 1e-8, 500, sparse_matrix<double>::jacobi);
	for(auto ix: x) {
		std::cout << ix << " ";
	}
	std::cout << std::endl;
	return 0;
}