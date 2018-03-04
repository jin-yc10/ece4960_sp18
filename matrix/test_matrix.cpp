//
// Created by yj374 on 2/15/18.
//

#include "sparse_matrix.h"

#include <iostream>

int main() {
	// Note04, Pg11, Hacker Practice
	sparse_matrix<float> m = sparse_matrix<float>::generate_test_matrix();
	dense_matrix<float> dm(5);
	m.report();
	
	m.add_element(2,3,100.0);
	m.report();

  dm.copy(m);
  dm.report();
	
	float raw_x[5] = {5,4,3,2,1};
	std::vector<float> x(raw_x, raw_x + sizeof(raw_x) / sizeof(raw_x[0]) );
	auto b = m.product_ax(x);
	
	for(int i=0; i<b.size(); i++) {
		std::cout << b[i] << " ";
	}
  std::cout << std::endl;

  auto b2 = dm.product_ax(x);
  for(int i=0; i<b2.size(); i++) {
    std::cout << b2[i] << " ";
  }
	std::cout << std::endl << std::endl;

  m.row_permute(x, 0, 4);
  std::cout << "x after row_permute: ";
  for(int i=0; i<x.size(); i++) {
    std::cout << x[i] << " ";
  }
  std::cout << std::endl;
	m.report();

  m.row_scale(x, 0,4, 0.5f);
  std::cout << "x after row_permute: ";
  for(int i=0; i<x.size(); i++) {
    std::cout << x[i] << " ";
  }
  std::cout << std::endl;
  m.report();

	return 0;
}