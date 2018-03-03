//
// Created by 金宇超 on 2/15/18.
//

#ifndef ECE4960_SP18_MATRIX_H
#define ECE4960_SP18_MATRIX_H

#include <vector>
#include <stdio.h>
#include <iostream>
#include <cmath>
#include <utils/mem_usage.h>
#include <matrix/dense_matrix.h>

template<class T>
class dense_matrix;

template<class T>
class sparse_matrix {
private:
	std::vector<T> vals;
	std::vector<int> row_ptr;
	std::vector<int> col_ind;
public:
	sparse_matrix<T>(int rank) {
	  row_ptr.resize(rank);
	}

	T norm_diff(dense_matrix<T> d) {
		T sum = 0;
		for( int r=0; r<rank(); r++) {
			for( int c=0; c<rank(); c++) {
				T v = retrieve_element(r, c);
				T v_ = d.retrieve_element(r, c);
				sum += (v-v_)*(v-v_);
			}
		}
		return std::sqrt(sum);
	}

	void add_element(int rowId, int colId, T val) {
		if( val == 0.0 ) { // TODO: this comparision might have problem
			return; // add a zero, directly jump out
		}
		int row_idx = row_ptr[rowId];
		int col_idx = -1;
		int i;
		for( i=row_idx; i<row_ptr[rowId+1]; i++) {
			if( col_ind[i] == colId ) {
				vals[i] = val; // already exist, replace it
				return;
			} else if(col_ind[i] > colId) {
        break;
      }
		}

		vals.insert(vals.begin()+i, val);
		col_ind.insert(col_ind.begin()+i, colId);
		for( int r=rowId+1; r<this->rank()+1; r++) {
			row_ptr[r]++;
		}
	}
	
	void delete_element(int rowId, int colId) {
		int row_idx = row_ptr[rowId];
		int col_idx = -1;
		int i=row_idx;
		for( i=row_idx; i<row_ptr[rowId+1]; i++) {
			if( col_ind[i] == colId ) {
				
				return;
			}
		}
	}
	
	T retrieve_element(int rowId, int colId) {
		int row_idx = row_ptr[rowId];
		int col_idx = -1;
		for( int i=row_idx; i<row_ptr[rowId+1]; i++) {
			if( col_ind[i] == colId ) {
				return vals[i];
			}
		}
		return (T)0;
	}

  int get_row_idx(int row_id) const {
    return row_ptr[row_id];
  }

  T get_element_by_id(int id) const {
    return vals[id];
  }

  int get_col_by_id(int id) const {
    return col_ind[id];
  }
	
	int rank() {
		return (int)row_ptr.size()-1;
	}
	
	int element_cnt() {
		return (int)vals.size();
	}
	
	std::vector<T> product_ax(std::vector<T> x) {
		if( x.size() != rank() ) {
			return std::vector<T>();
		}
		std::vector<T> b(x.size());
		for( int r=0; r<rank(); r++) {
			int row_idx = row_ptr[r];
			T sum = (T)0;
			for( int i=row_idx; i<row_ptr[r+1]; i++) {
				T v = vals[i];
				int c = col_ind[i];
				sum += v*x[c];
			}
			b[r] = sum;
		}
		return b;
	}

  int load_mtx(const char* path) {
    FILE* pf = fopen(path, "r");// data/memplus.mtx","r");
    char buf[500];
    fgets(buf, 500, pf);
    int row_rank, col_rank, nNZ;
    int r, c;
    float v;
    int n = 0;
    fscanf(pf, "%d %d %d", &row_rank, &col_rank, &nNZ);

    this->row_ptr.resize(row_rank+1);

    std::vector<std::vector<T>> value_buf(row_rank);
    std::vector<std::vector<int>> col_buf(row_rank);

    while(!feof(pf)) {
      fscanf(pf, "%d %d %f\n", &r, &c, &v);
      n++;
      value_buf[r-1].push_back(v);
      col_buf[r-1].push_back(c-1);
    }

    int row_idx = 0;
    this->row_ptr[0] = row_idx;
    this->vals.clear();
    this->col_ind.clear();
    for(int r_id=1; r_id<row_rank+1; r_id++) {
      if(r_id%2000==0) {
        printf("Row %d, Cnt=%d, Memory:", r_id-1, row_idx);
        report_mem_usage();
      }
      row_idx += value_buf[r_id-1].size();
      this->row_ptr[r_id] = row_idx;
      for(int c_id=0; c_id<value_buf[r_id-1].size(); c_id++) {
        this->vals.push_back(value_buf[r_id-1][c_id]);
        this->col_ind.push_back(col_buf[r_id-1][c_id]);
      }
    }

    fclose(pf);

    value_buf.clear();
    col_buf.clear();

    std::cout << "Total: " << n << std::endl;
    return n;
  }

  int row_permute(std::vector<T>& x, int row_i, int row_j) {
    std::swap(x[row_i], x[row_j]);
    if(row_i > row_j) {
      int temp = row_i; row_i = row_j; row_j = temp; // make sure row_i < row_B
    }
    int n_A = row_ptr[row_i+1] - row_ptr[row_i];
    int n_B = row_ptr[row_j+1] - row_ptr[row_j];
    std::vector<int> _row_idx;
    std::vector<T> _vals;
    std::vector<int> _col_idx;
    for(int r=0; r<rank(); r++) {
      _row_idx.push_back(_vals.size());
      if(r == row_i) {
        int _r = row_j;
        for(int c=row_ptr[_r]; c<row_ptr[_r+1]; c++){
          _vals.push_back(vals[c]);
          _col_idx.push_back(col_ind[c]);
        }
      } else if(r == row_j) {
        int _r = row_i;
        for(int c=row_ptr[_r]; c<row_ptr[_r+1]; c++){
          _vals.push_back(vals[c]);
          _col_idx.push_back(col_ind[c]);
        }
      } else {
        for (int c = row_ptr[r]; c < row_ptr[r + 1]; c++) {
          _vals.push_back(vals[c]);
          _col_idx.push_back(col_ind[c]);
        }
      }
    }
    _row_idx.push_back(_vals.size());
    row_ptr = _row_idx;
    vals = _vals;
    col_ind = _col_idx;
	  return 0;
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
		printf("r:");
		for( int r=0; r<rank()+1; r++) {
			printf("%d ", row_ptr[r]);
		}
		printf("\n");
		
		printf("c:");
		for( int r=0; r<element_cnt(); r++) {
			printf("%d ", col_ind[r]);
		}
		printf("\n");
		
		printf("v:");
		for( int r=0; r<element_cnt(); r++) {
			printf("%f ", vals[r]);
		}
		printf("\n");
		
		for( int r=0; r<rank(); r++) {
			for( int c=0; c<rank(); c++) {
				printf("%.02f ", this->retrieve_element(r,c));
			}
			printf("\n");
		}
		printf("\n");
	}
	
	static sparse_matrix generate_test_matrix() {
		sparse_matrix<T> m(5);
		for (int i = 1; i <= 12; i++) {
			m.vals.push_back((T) i);
		}
    m.row_ptr.clear();
		m.row_ptr.push_back(0);
		m.row_ptr.push_back(3);
		m.row_ptr.push_back(6);
		m.row_ptr.push_back(9);
		m.row_ptr.push_back(10);
		m.row_ptr.push_back(12);
		
		int col_raw[] = {0, 1, 4,
		                 0, 1, 2,
		                 1, 2, 4,
		                 3,
		                 0, 4};
		m.col_ind = std::vector<int>(col_raw, col_raw + sizeof(col_raw) / sizeof(col_raw[0]) );
		return m;
	}
};


#endif //ECE4960_SP18_MATRIX_H
