//
// Created by yj374 on 2/25/2018.
//
#include <iostream>
#include "matrix/sparse_matrix.h"

int main(int argc, char** argv) {
  sparse_matrix<float> m = sparse_matrix<float>::generate_test_matrix();
  dense_matrix<float> d(m);
  float raw_x[5] = {5,4,3,2,1};
  std::vector<float> x_sparse(raw_x, raw_x + sizeof(raw_x) / sizeof(raw_x[0]) );
  std::vector<float> x_dense(raw_x, raw_x + sizeof(raw_x) / sizeof(raw_x[0]) );

  m.report(); d.report();
  m.row_permute(x_sparse, 0, 2);
  d.row_permute(x_dense, 0, 2);

  std::cout << "Diff = " << m.norm_diff(d) << std::endl;

  m.report(); d.report();
  m.row_permute(x_sparse, 0, 4);
  d.row_permute(x_dense, 0, 4);

  m.report(); d.report();
  std::cout << "Diff = " << m.norm_diff(d) << std::endl;

  m.row_scale(x_sparse, 0, 3, 3.0f);
  d.row_scale(x_dense, 0, 3, 3.0f);
  m.report(); d.report();
  std::cout << "Diff = " << m.norm_diff(d) << std::endl;

  m.row_scale(x_sparse, 4, 1, -4.4f);
  d.row_scale(x_dense, 4, 1, -4.4f);
  m.report(); d.report();
  std::cout << "Diff = " << m.norm_diff(d) << std::endl;

  auto b_sparse = m.product_ax(x_sparse);
  auto b_dense = d.product_ax(x_dense);
  float diff = 0.0;
  for(int i=0; i<b_sparse.size(); i++) {
    float __d = b_sparse[i] - b_dense[i];
    diff += __d * __d;
  }
  std::cout << "Ax=b, diff = " << std::sqrt(diff) << std::endl;

  return 0;
}