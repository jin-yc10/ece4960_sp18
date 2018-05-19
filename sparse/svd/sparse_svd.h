//
// Created by yj374 on 5/14/18.
//

#ifndef ECE4960_SP18_SPARSE_SVD_H
#define ECE4960_SP18_SPARSE_SVD_H

#include <sparse/matrix/sparse_matrix.h>

#include <numeric>

namespace sparse {

struct jacobi_rotation {
	double c, s;
	int p, q;
};

class svd {
public:
  const double eps = 1e-8;
  int rows, cols;
  sparse_matrix<double> *A, *U, *V, *B, *J;
  std::vector<double> E;
  std::vector<double> Es;

  svd(sparse_matrix<double>* m) {
	  A = m;
	  rows = A->rows;
	  cols = A->cols;
	  B = new sparse_matrix<double>(rows, rows);
	  J = new sparse_matrix<double>(rows, rows);
	  // B = A * A.T
	  for (int i = 0; i < rows; i++) {
		  J->add_element(i, i, 1.0); // set J as eye matrix
		  double val = 0.0;
		  int row_start_i = m->get_row_idx(i);
		  int row_end_i = m->get_row_idx(i + 1);
		  for (int j = i; j < rows; j++) {
			  int row_start_j = m->get_row_idx(j);
			  int row_end_j = m->get_row_idx(j+1);
			  int tmp_i = row_start_i;
			  int tmp_j = row_start_j;
			  val = 0.0;
			  while (tmp_i != row_end_i && tmp_j != row_end_j) {
				  int col_i = m->get_col_by_id(tmp_i);
				  int col_j = m->get_col_by_id(tmp_j);
				  if (col_i == col_j) {
					  val += m->get_element_by_id(tmp_i) * m->get_element_by_id(tmp_j);
					  tmp_i++; tmp_j++;
				  } else {
					  if (col_i > col_j) {
						  tmp_j++;
					  } else {
						  tmp_i++;
					  }
				  }
			  }
			  if (std::abs(val) > eps) {
				  B->add_element(i, j, val);
				  if (i != j) {
					  B->add_element(j, i, val);
				  }
			  } 
		  }
	  }
	  E.resize(rows);
  }

  void apply_jacobiT_left(sparse_matrix<double>* m, const jacobi_rotation& j) {
	  // m = GT*m
	  std::vector<double> tmp1(rows, 0.0);
	  std::vector<double> tmp2(rows, 0.0);
	  int row_start = m->get_row_idx(j.p);
	  int row_end = m->get_row_idx(j.p+1);
	  for (int x = row_start; x < row_end; x++) {
		  int col_idx = m->get_col_by_id(x);
		  double val = m->get_element_by_id(x);
		  tmp1[col_idx] += j.c * val;
		  tmp2[col_idx] += -j.s * val;
	  }
	  row_start = m->get_row_idx(j.q);
	  row_end = m->get_row_idx(j.q + 1);
	  for (int x = row_start; x < row_end; x++) {
		  int col_idx = m->get_col_by_id(x);
		  double val = m->get_element_by_id(x);
		  tmp1[col_idx] += j.s * val;
		  tmp2[col_idx] += j.c * val;
	  }
	  for (int ix = 0; ix < rows; ix++) {
		  /*std::cout << tmp1[ix] << " " << tmp2[ix] << std::endl;*/
		  //std::cout << tmp2[ix] << " " << (*m)(j.p, ix) * -j.s + (*m)(j.q, ix) * j.c << std::endl;
		  m->add_element(j.p, ix, tmp1[ix]);
		  m->add_element(j.q, ix, tmp2[ix]);
	  }
  }

  void apply_jacobi_right(sparse_matrix<double>* m, const jacobi_rotation& j) {
	  // m = m*G
	  for (int i = 0; i < rows; i++) {
		  int row_start_i = m->get_row_idx(i);
		  int row_end_i = m->get_row_idx(i + 1);
		  double val1 = 0.0;
		  double val2 = 0.0;
		  for (int c = row_start_i; c < row_end_i; c++) {
			  int col = m->get_col_by_id(c);
			  if (col == j.p) {
				  val1 += m->get_element_by_id(c) * j.c;
				  val2 += m->get_element_by_id(c) * -j.s;
			  }
			  else if (col == j.q) {
				  val1 += m->get_element_by_id(c) * j.s;
				  val2 += m->get_element_by_id(c) * j.c;
			  }
		  }
		  if (std::abs(val1) > eps) {
			  m->add_element(i, j.p, val1);
			  m->add_element(i, j.q, val2);
		  }
	  }
  }

  int rotate(int i, int j) {
    double ele = (*B)(i, j);
    if( std::abs(ele) < eps ) {
      return 1;
    } else {
      double ele1 = (*B)(i, i); double ele2 = (*B)(j, j);
      double tao = (ele1 - ele2) / (2 * ele);
      double sign = tao > 0 ? 1:-1;
      double t = sign / (std::abs(tao)+std::sqrt(1+tao*tao));
      double c = 1 / std::sqrt(1+t*t);
      double s = c*t;
      jacobi_rotation jr; jr.c = c; jr.s = s; jr.p = i; jr.q = j;
	  /*std::cout << "Rotate " << i << " " << j << " c = " << c << " s = " << s << std::endl;*/
      apply_jacobi_right(B, jr);
      apply_jacobiT_left(B, jr);
      apply_jacobi_right(J, jr);	  
	  return 0;
    }
  }

  template <typename T>
  std::vector<size_t> sort_indexes(const std::vector<T> &v) {

	  // initialize original index locations
	  std::vector<size_t> idx(v.size());
	  std::iota(idx.begin(), idx.end(), 0);

	  // sort indexes based on comparing values in v
	  std::sort(idx.begin(), idx.end(),
		  [&v](size_t i1, size_t i2) {return v[i1] > v[i2]; });

	  return idx;
  }

  void calc() {
    int iter_num = 30;
    int flag = 1;
    while( iter_num > 0 ) {
		//std::cout << "Iter " << 30 - iter_num << std::endl;
		//B->report(false);
		iter_num --;
		flag = 1;
		for(int p = 0; p < rows; p++) {
			for(int q = p+1; q < rows; q++) {
				int f_ = rotate(p, q);
				flag = flag && f_;
			}
		}
		if( flag ) {
			break;
		}
    }
    for(int i=0; i < rows; i++ ) {
		double val = (*B)(i, i);
		if(std::abs(val) < eps) {
			E[i] = 0;
		} else {
			E[i] = std::sqrt(std::abs(val));
		}
    }
	auto idxE = sort_indexes(E);
	for (auto i : idxE) {
		Es.push_back(E[i]);
	}
  }

};
}; // namespace sparse


#endif //ECE4960_SP18_SPARSE_SVD_H
