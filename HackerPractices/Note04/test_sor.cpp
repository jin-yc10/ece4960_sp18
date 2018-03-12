//
// Created by yj374 on 3/11/18.
//

#include <matrix/sparse_matrix.h>
#include <matrix/dense_matrix.h>

// Hacker Practice, Note04, Pg55

int main(int argc, char** argv) {
	std::vector<double> vals = {-4, 1, 1, 4, -4, 1, 1, -4, 1, 1, -4, 1, 1, -4, 1, 1, 1, -4};
	std::vector<double> row_ptr = {0, 3, 6, 9, 12, 15};
	std::vector<double> col_idx = {0, 1, 4, 0, 1, 2, 1, 2, 3, 2, 3, 4, 0, 3, 4};
	sparse_matrix<double> s = sparse_matrix<double>::generate_test_matrix(vals, row_ptr, col_idx);
	s.report();
	
	std::vector<double> b = {1, 0, 0, 0, 0};
	double w = 0.5;
	std::cout << "w = " << w << std::endl;
	auto x = s.solve(b, 1e-8, 500, sparse_matrix<double>::sor, (void*)&w);
	auto b_ = s.product_ax(x);
	for(auto ib: b_) {
		std::cout << ib << " ";
	}
	std::cout << std::endl;
}