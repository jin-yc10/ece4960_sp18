//
// Created by yj374 on 3/7/18.
//

#include <matrix/sparse_matrix.h>
#include <matrix/vector_util.h>
#include <utils/timer_util.h>

/// validate one case
/// \param sp input sparse matrix
/// \param de input dense matrix
/// \param b input vector b
void validate(sparse_matrix<double>& sp,
							dense_matrix<double>& de,
							std::vector<double>& b) {
	timer_util stopwatch;
	stopwatch.tick();
	auto x = sp.solve(b, 1e-12, 100, sparse_matrix<double>::jacobi);
	stopwatch.tock("sp.solve.jacobi took");
	std::cout << std::endl;
	
	stopwatch.tick();
	auto x2 = sp.solve(b, 1e-12, 100, sparse_matrix<double>::gauss_seidel);
	stopwatch.tock("sp.solve.gauss_seidel took");
	std::cout << std::endl;
	
	stopwatch.tick();
	double w = 0.9;
	auto x3 = sp.solve(b, 1e-12, 100, sparse_matrix<double>::sor, (void*)&w);
	stopwatch.tock("sp.solve.sor took");
	std::cout << std::endl;
	
	stopwatch.tick();
	auto dx = de.solve(b, 1e-12, 100, dense_matrix<double>::gauss_seidel);
	stopwatch.tock("de.solve took");
	std::cout << std::endl;
	
	auto b_ = sp.product_ax(x);
	std::cout << "x[0:100] = ";
	for(int i=0; i<100; i++) {
		std::cout << x[i] << " ";
	}
	std::cout << std::endl;
	std::cout << "b[0:100] = ";
	for(int i=0; i<100; i++) {
		std::cout << b_[i] << " ";
	}
	std::cout << std::endl;
	std::cout << "||Ax-b|| = " << norm_vector_diff(b, b_) << std::endl;
}

int main(int argc, char** argv) {
	sparse_matrix<double> sp(5000);
	if( argc == 2 )
		sp.load_csv(argv[1], 5000);
	
	dense_matrix<double> de(5000);
	de.copy(sp); // copy from sparse matrix
	
	std::vector<double> b(5000);
	
	// case 1, b = [1, 0, 0....0], 1st element is 1, others are 0
	for(int i=0; i<5000; i++) {
		if( i == 0 ) {
			b[i] = 1.0;
		} else {
			b[i] = 0.0;
		}
	}
	validate(sp, de, b);
	
	// case 2, b = [0,0,0,0,1,0,0,...,0], 5th element is 1, others are 0
	for(int i=0; i<5000; i++) {
		if( i == 4 ) {
			b[i] = 1.0;
		} else {
			b[i] = 0.0;
		}
	}
	validate(sp, de, b);
	
	// case 3, b = [1, 1, 1, 1, ..., 1], all elements are 1
	for(int i=0; i<5000; i++) {
		b[i] = 1.0;
	}
	validate(sp, de, b);

	return 0;
}
