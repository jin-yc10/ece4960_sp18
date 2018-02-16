//
// Created by 金宇超 on 2/15/18.
//

#include "sparse_matrix.h"

#include <iostream>

int main() {
	sparse_matrix<float> m = sparse_matrix<float>::generate_test_matrix();
	m.report();
	
	m.add_element(2,3,100.0);
	m.report();
	
	float raw_x[5] = {5,4,3,2,1};
	std::vector<float> x(raw_x, raw_x + sizeof(raw_x) / sizeof(raw_x[0]) );
	auto b = m.product_ax(x);
	
	for(int i=0; i<b.size(); i++) {
		std::cout << b[i] << " ";
	}
	std::cout << std::endl;
	
	return 0;
}