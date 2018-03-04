//
// Created by yj374 on 3/4/18.
//

#include <matrix/sparse_matrix.h>
#include <matrix/dense_matrix.h>

/*
Result was:
Dense A1 = 24, Sparse A1 = 24
Dense A_inf = 24, Sparse A_inf = 24

Check with Matlab:
A = [1.00 2.00 0.00 0.00 3.00
4.00 5.00 6.00 0.00 0.00
0.00 7.00 8.00 0.00 9.00
0.00 0.00 0.00 10.00 0.00
11.00 0.00 0.00 0.00 12.00];
 norm1 = norm(A, 1) % 24
 normi = norm(A, inf) % 24
 */

int main(int argc, char** argv) {
	dense_matrix<double> d(5);
	sparse_matrix<double> s = sparse_matrix<double>::generate_test_matrix();
	d.copy(s);
	
	double dense_A1 = 0, sparse_A1 = 0;
	double dense_A_inf = 0, sparse_A_inf = 0;
	
	// Calculate A1
	for(int i=0; i<5; i++) {
		double T = 0.0;
		double T2 = 0.0;
		for(int r=0; r<5; r++ ) {
			T += std::abs(d.retrieve_element(r,i));
			T2 += std::abs(s.retrieve_element(r,i));
		}
		dense_A1 = std::max(dense_A1, T);
		sparse_A1 = std::max(sparse_A1, T2);
	}
	std::cout << "Dense A1 = " << dense_A1 << ", Sparse A1 = " << sparse_A1 << std::endl;
	
	for(int i=0; i<5; i++) {
		double T = 0.0;
		double T2 = 0.0;
		for(int c=0; c<5; c++ ) {
			T += std::abs(d.retrieve_element(i, c));
			T2 += std::abs(s.retrieve_element(i, c));
		}
		dense_A_inf = std::max(dense_A_inf, T);
		sparse_A_inf = std::max(sparse_A_inf, T2);
	}
	std::cout << "Dense A_inf = " << dense_A_inf << ", Sparse A_inf = " << sparse_A_inf << std::endl;
	
	return 0;
}