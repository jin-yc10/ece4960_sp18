//
// Created by yj374 on 3/4/18.
//

#include <matrix/dense_matrix.h>

/*
 1.00 2.00 0.00 0.00 3.00
 4.00 5.00 6.00 0.00 0.00
 0.00 7.00 8.00 0.00 9.00
 0.00 0.00 0.00 10.00 0.00
11.00 0.00 0.00 0.00 12.00
 */
int main(int argc, char** argv) {
	
	// Initialize the matrix
	dense_matrix<double> d(5);
	sparse_matrix<double> s = sparse_matrix<double>::generate_test_matrix();
	d.copy(s);
	d.report();
	
	double raw_b[5] = {5,4,3,2,1};
	std::vector<double> b(raw_b, raw_b + sizeof(raw_b) / sizeof(raw_b[0]) );
	
	// Find minimum fill-in
	int min_fill_in_idx[5] = {0};
	for(int c=0; c<5; c++) {
		int min_fill_ins = 5;
		for(int r1=c; r1<5; r1++) {
			int fill_ins = 0;
			if( d.retrieve_element(r1, c) != 0 ) {
				for(int r2=c; r2<5; r2++) {
					if( r2 == r1 ) continue; // skip same row
					if( d.retrieve_element(r2, c) != 0 ) {
						for( int c2 = c+1; c2<5; c2++) {
							if( d.retrieve_element(r1, c2) != 0 && d.retrieve_element(r2, c2) == 0 ) {
								fill_ins += 1;
							}
						}
					} else {
						continue;
					}
				}
				if( fill_ins < min_fill_ins ) {
					min_fill_ins = fill_ins;
					min_fill_in_idx[c] = r1;
				}
			} else {
				continue;
			}
		}
		// Here we directly permute the rows
		// After here, we can use the permuted matrix directly
		d.row_permute(b, c, min_fill_in_idx[c]);
		
		// With report, we can see how the matrix permute during the iterations
		d.report();
		std::cout << min_fill_ins << std::endl;
	}
	for(int i=0; i<5; i++)
		std::cout << min_fill_in_idx[i] << " ";
	std::cout << std::endl;
	
	// Calculate the actual upper triangle matrix
	for(int c=0; c<5; c++) {
		int idx = c;
		for(int r=c+1; r<5; r++) {
			if( d.retrieve_element(r, c) != 0 ) {
				double scale = d.retrieve_element(r, c)/d.retrieve_element(idx, c);
				d.row_scale(b, idx, r, -scale);
			}
		}
		d.report();
	}
	
	// Backward substitution
	// x = {-0.183908 0.2 -1.71944 2.63009 0.291536 }
	std::cout << "x = ";
	double x[5] = {0.0};
	for(int c=4; c>=0; c--) {
		double b_ = b[c];
		for(int c2=4; c2>c; c2--) {
			b_ -= x[c2]*d.retrieve_element(c, c2);
		}
		x[c] = b_ / d.retrieve_element(c, c);
		std::cout << x[c] << " ";
	}
	std::cout << std::endl;
	
	// Check the result with the permuted matrix
	std::vector<double> xv(x, x + sizeof(x) / sizeof(x[0]) );
	auto b_ = d.product_ax(xv);
	// b_ = {1 3.63636 -2.09091 2 16}
	std::cout << "b_ = ";
	for(int i=0; i<5; i++) {
		std::cout << b_[i] << " ";
	}
	// b = { 1 3.63636 -2.09091 2 16 } == b_
	std::cout << std::endl;
	std::cout << "b = ";
	for(int i=0; i<5; i++) {
		std::cout << b[i] << " ";
	}
	std::cout << std::endl;
	
	// Check the answer vector x with the original matrix
	dense_matrix<double> d_(5);
	sparse_matrix<double> s_ = sparse_matrix<double>::generate_test_matrix();
	d_.copy(s_);
	b_ = d_.product_ax(xv);
	// b_ = { 5 4 3 2 1 } this time, same as the input vector
	// So the X we got was correct
	std::cout << "b = ";
	for(int i=0; i<5; i++) {
		std::cout << b_[i] << " ";
	}
	std::cout << std::endl;
	
	return 0;
}