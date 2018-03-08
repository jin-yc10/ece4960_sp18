//
// Created by 金宇超 on 3/7/18.
//

#ifndef ECE4960_SP18_TEST_MATRIX_H
#define ECE4960_SP18_TEST_MATRIX_H

#include "sparse_matrix.h"
#include "dense_matrix.h"

class matrix_test_suite {
	sparse_matrix<double> sp;
	dense_matrix<double> de;
public:
	matrix_test_suite() : sp(5), de(5){
		sp = sparse_matrix<double>::generate_test_matrix();
		de.copy(sp);
	}
	
	void test_sparse_matrix_init() {
		sp.report();
		de.report();
		std::cout << "matrix_init: ||sp-de|| = " << sp.norm_diff(de) << std::endl;
	}
	
	void test_add_element() {
		sp.add_element(3, 4, 5.0);
		de.add_element(3, 4, 5.0);
		std::cout << "add_element: ||sp-de|| = " << sp.norm_diff(de) << std::endl;
	}
	
	void test_retrieve_element() {
		double v1 = sp.retrieve_element(3, 4);
		double v2 = de.retrieve_element(3, 4);
		std::cout << "retrieve_element: |v1-v2| = " << std::abs(v1-v2) << std::endl;
	}
	
	void test_product_ax() {
		double raw_x[5] = {5,4,3,2,1};
		std::vector<double> x(raw_x, raw_x + sizeof(raw_x) / sizeof(raw_x[0]) );
		auto b1 = sp.product_ax(x);
		auto b2 = de.product_ax(x);
		std::cout << "product_ax: ||b1-b2|| = " << norm_vector_diff(b1,b2) << std::endl;
	}
	
	void test_row_permute() {
		double raw_x[5] = {5,4,3,2,1};
		std::vector<double> x(raw_x, raw_x + sizeof(raw_x) / sizeof(raw_x[0]) );
		sp.row_permute(x, 1, 3);
		de.row_permute(x, 1, 3);
		std::cout << "row_permute: ||sp-de|| = " << sp.norm_diff(de) << std::endl;
	}
	
	void test_row_scale() {
		double raw_x[5] = {5,4,3,2,1};
		std::vector<double> x(raw_x, raw_x + sizeof(raw_x) / sizeof(raw_x[0]) );
		sp.row_scale(x, 1, 3, 2.0);
		de.row_scale(x, 1, 3, 2.0);
		std::cout << "row_scale: ||sp-de|| = " << sp.norm_diff(de) << std::endl;
	}
};

#endif //ECE4960_SP18_TEST_MATRIX_H
