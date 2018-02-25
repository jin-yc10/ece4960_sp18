//
// Created by yj374 on 2/25/2018.
//

#include <cstdio>
#include <matrix/sparse_matrix.h>
#include "../../utils/mem_usage.h"
#include "../../utils/timer_util.h"

int main(int argc, char** argv) {

  timer_util stopwatch;
  stopwatch.tick();

  sparse_matrix<double> mat(0);
  std::cout << argv[1] << std::endl;
  int n_elements = mat.load_mtx(argv[1]);

  stopwatch.tock("Load data: ");

  std::cout << "A[0,0] = " << mat.retrieve_element(0,0) << std::endl;
  std::cout << "A[6838,6838] = " << mat.retrieve_element(6838,6838) << std::endl;
  std::cout << "A[17757,17757 = " << mat.retrieve_element(17757,17757) << std::endl;

//  std::cout << sizeof(mat) << std::endl;
  stopwatch.tick();
  std::vector<double> x(mat.rank());
  mat.row_permute(x, 0, 2);
  mat.row_permute(x, 0, 4);
  mat.row_permute(x, 9, 2999);
  mat.row_permute(x, 4999, 9999);
  mat.row_permute(x, 5, 14999);
  stopwatch.tock("5 permute: ");

  stopwatch.tick();
  mat.row_scale(x, 1, 3, 3.0);
  mat.row_permute(x, 1, 4);
  mat.row_scale(x, 4, 3, -3.0);
  stopwatch.tock("scale, permute then scale: ");

  std::fill(x.begin(), x.end(), 1.0f);
  auto b = mat.product_ax(x);

  double sum_bj = 0.0f;
  double sum_a = 0.0f;
  for(int r=0; r<mat.rank(); r++) {
    sum_bj += b[r];
    int low = mat.get_row_idx(r);
    int high = mat.get_row_idx(r+1);
    for(int id=low; id<high; id++) {
      double v = mat.get_element_by_id(id);
      sum_a += v;
    }
  }
  std::cout << "Sigma B = "<< sum_bj << ", Sigma A = " << sum_a << ", Delta = " << sum_bj - sum_a << std::endl;
  std::cout.flush();

  report_mem_usage();
  int not_used;
  std::cin >> not_used;
  return 0;
}