//
// Created by 金宇超 on 2/15/18.
//

#ifndef ECE4960_SP18_DENSE_MATRIX_H
#define ECE4960_SP18_DENSE_MATRIX_H

#include <matrix/sparse_matrix.h>

template<class T>
class sparse_matrix;

template<class T>
class dense_matrix {

  std::vector<std::vector<T>> vals;

  void init(int rank) {
    vals.clear();
    for(int r=0; r<rank; r++) {
      vals.push_back(std::vector<T>());
      for(int c=0; c<rank; c++) {
        vals[r].push_back((T)0);
      }
    }
  }

public:
  dense_matrix<T>(int rank) {
    init(rank);
  }

  dense_matrix<T>(sparse_matrix<T> s) {
    copy(s);
  }

  void copy(sparse_matrix<T>& sp) {
    init(sp.rank());
    for(int r=0; r<rank(); r++) {
      int r_begin = sp.get_row_idx(r);
      int r_end = sp.get_row_idx(r+1);
      for(int c=r_begin; c<r_end; c++) {
        T v = sp.get_element_by_id(c);
        int col = sp.get_col_by_id(c);
        this->vals[r][col] = v;
      }
    }
  }

  ~dense_matrix() {

  }

  void add_element(int rowId, int colId, T val) {
    vals[rowId][colId] = val;
  }

  int rank() {
    return (int)vals.size();
  }

  int element_cnt() {
    return rank()*rank();
  }

  T retrieve_element(int rowId, int colId) {
    return vals[rowId][colId];
  }

  std::vector<T> product_ax(std::vector<T> x) {
    if( x.size() != rank() ) {
      return std::vector<T>();
    }
    std::vector<T> b(x.size());
    for( int r=0; r<rank(); r++) {
      T sum = (T)0;
      for( int c=0; c<rank(); c++) {
        T v = vals[r][c];
        sum += v*x[c];
      }
      b[r] = sum;
    }
    return b;
  }

  int row_permute(std::vector<T>& x, int row_i, int row_j) {
    std::swap(x[row_i], x[row_j]);
    for(int c=0; c<rank(); c++) {
      std::swap(vals[row_i][c], vals[row_j][c]);
    }
  }

  int row_scale(std::vector<T>& x, int row_i, int row_j, T a) {
    x[row_j] += x[row_i]*a;
    for(int c=0; c<rank(); c++) {
      T v_i = retrieve_element(row_i, c);
      if(v_i == (T)0) {
        continue;
      } else {
        T v_j = retrieve_element(row_j, c);
        add_element(row_j, c, v_j+v_i*a);
      }
    }
    return 0;
  }

  void report() {
    for( int r=0; r<rank(); r++) {
      for( int c=0; c<rank(); c++) {
        printf("%.02f ", this->retrieve_element(r,c));
      }
      printf("\n");
    }
    printf("\n");
  }
};


#endif //ECE4960_SP18_DENSE_MATRIX_H
