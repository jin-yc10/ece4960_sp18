//
// Created by yj374 on 2/15/18.
//

#ifndef ECE4960_SP18_MATRIX_H
#define ECE4960_SP18_MATRIX_H

#include <vector>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <utils/mem_usage.h>
#include <matrix/dense_matrix.h>
#include "vector_util.h"

template<class T>
class dense_matrix;

template<class T>
class sparse_matrix {
private:
	std::vector<T> vals;
	std::vector<int> row_ptr;
	std::vector<int> col_ind;
	
	bool preconditioning() {
		return true;
	}
	
	std::vector<T> initial_guess(const std::vector<T>& b) {
		std::vector<T> x(rank());
		return x;
	}
	
public:
	enum solve_method {
		jacobi, gauss_seidel, sor
	};
	
	sparse_matrix<T>(int rank) {
	  row_ptr.resize(rank);
	}

	/// val = || mat_1 - mat_2 ||, the rank of the two matrix should be the same
	/// mat_1 is this and mat_2 is a dense matrix
	/// \param d dense matrix D
	/// \return the norm_diff value of the two matrix
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

	/// add an element to the matrix, if the val is zero, the function will directly return
	/// and this function will replace the old value with the new value
	/// \param rowId
	/// \param colId
	/// \param val
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
	
	/// NOT USED
	/// \param rowId
	/// \param colId
	void delete_element(int rowId, int colId) {
		int row_idx = row_ptr[rowId];
		int col_idx = -1;
		int i=row_idx;
		for( i=row_idx; i<row_ptr[rowId+1]; i++) {
			if( col_ind[i] == colId ) {
				// TODO: NOT FINISHED
				return;
			}
		}
	}
	
	/// Return the value at [rowId, colId]
	/// \param rowId
	/// \param colId
	/// \return
	T retrieve_element(int rowId, int colId) {
		int row_idx = row_ptr[rowId];
		int col_idx = -1;
		for( int i=row_idx; i<row_ptr[rowId+1]; i++) {
			if( col_ind[i] == colId ) {
				// TODO: USE BI-SEARCH to accelerate?
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
	
	/// return the vector of Ax, if rank doesn't match, return empty vector
	/// \param x x, should be a vector, size of x should be equal to A.rank()
	/// \return a vector, Ax
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

	/// Load the sparse matrix from csv file
	/// \param folder the folder contains colInd.csv, rowPtr.csv, value.csv
	/// \param rank the rank
	/// \return number of non-zero elements
	int load_csv(const char* folder, int rank) {
		std::cout << "Load csv from " << folder << ", rank = " << rank << std::endl;
		
		std::string prefix(folder);
		
		std::ifstream row_file(prefix+"/rowPtr.csv");
		std::ifstream col_file(prefix+"/colInd.csv");
		std::ifstream val_file(prefix+"/value.csv");
		
		this->row_ptr.clear();
		this->vals.clear();
		this->col_ind.clear();
		
		this->row_ptr.resize(rank+1);
		for(int i=0; i<rank+1; i++) {
			int row_idx;
			row_file >> row_idx;
			this->row_ptr[i] = row_idx-1;
		}
		
		int nz = this->row_ptr[rank];
		this->vals.resize(nz);
		this->col_ind.resize(nz);
		
		for(int i=0; i<rank; i++) {
			int row_idx = this->row_ptr[i];
			int row_idx_next = this->row_ptr[i+1];
			for(int c=row_idx; c<row_idx_next; c++) {
				int col_id;
				col_file >> col_id;
				this->col_ind[c] = col_id - 1;
				val_file >> this->vals[c];
			}
		}
		
		std::cout << "Load done, nz = " << nz << " " << this->col_ind.size() << " " << this->vals.size() << std::endl;
		
		return nz;
	}
	
	std::vector<T> solve_sor(const std::vector<T>& b, T epsilon,
	                         unsigned int max_iter = 4, void* args = NULL) {
		T w = *(double*)args;
		std::vector<T> x(rank());
		std::vector<T> x_old(rank());
		for( int i=0; i<rank(); i++) {
			x[i] = 0.000001;
		}
		if( 0 > w || 2 < w ) {
			return x;
		}
		T d_;
		std::cout << "Solver = SOR" << std::endl;
		unsigned int iter = 0;
		while(max_iter > iter) {
			iter++;
			double diff = 0.0;
			// copy vector x
			for( int i=0; i<rank(); i++) {
				x_old[i] = x[i];
			}
			std::cout << "Iter = " << iter;
			for( int j=0; j<rank(); j++) {
				d_ = b[j];
				int row_idx = this->row_ptr[j];
				int row_idx_next = this->row_ptr[j+1];
				for( int i=row_idx; i<row_idx_next; i++) {
					if( this->col_ind[i] == j ) continue; // skip aii
					if( this->col_ind[i] < j ) {
						d_ -= this->vals[i]*x[this->col_ind[i]];
					} else {
						d_ -= this->vals[i]*x_old[this->col_ind[i]];
					}
				}
				double t = d_/retrieve_element(j,j) - x_old[j]; // d_ = bi-sigma
				x[j] = x_old[j] + w*t;
				diff += (x[j] - x_old[j])*(x[j] - x_old[j]);
			}
			
			auto b_ = this->product_ax(x);
			auto diff_b = norm_vector_diff(b, b_);
			std::cout << "\t" << x_old[0] << "\t" << x[0] << "\t" << diff << "\t" << diff_b << std::endl;
			if( diff_b < epsilon ) {
				break;
			}
		}
		return x;
	}
	
	std::vector<T> solve_jacobi(const std::vector<T>& b, T epsilon,
	                            unsigned int max_iter = 4) {
		std::vector<T> x(rank());
		std::vector<T> x_old(rank());
		for( int i=0; i<rank(); i++) {
			x[i] = 1.0/this->retrieve_element(i, i)*b[i]; // init as D^-1*b
		}
		T d_;
		std::cout << "Solver = Jacobi" << std::endl;
		unsigned int iter = 0;
		while(max_iter > iter) {
			iter++;
			double diff = 0.0;
			for( int i=0; i<rank(); i++) {
				x_old[i] = x[i];
			}
//			std::cout << "Iter = " << iter;
			for( int j=0; j<rank(); j++) {
				d_ = 0.0;
				int row_idx = this->row_ptr[j];
				int row_idx_next = this->row_ptr[j+1];
				for( int i=row_idx; i<row_idx_next; i++) {
					if( this->col_ind[i] == j ) continue; // skip aii
					d_ = d_ + this->vals[i]*x_old[this->col_ind[i]];
				}
				double t = (b[j] - d_)/retrieve_element(j,j);
				diff += std::abs(x[j] - t);
				x[j] = t;
			}
			
			auto b_ = this->product_ax(x);
			auto diff_b = norm_vector_diff(b, b_);
//			std::cout << ", difference of b = " << diff_b << std::endl;
			if( diff < epsilon ) {
				break;
			}
		}
		return x;
	}
	
	std::vector<T> solve_gauss_seidel(const std::vector<T>& b, T epsilon,
	                            unsigned int max_iter = 4) {
		std::vector<T> x(rank());
		std::vector<T> x_old(rank());
		for( int i=0; i<rank(); i++) {
			x[i] = 0.000001;
		}
		T d_;
		std::cout << "Solver = Gauss-Seidel" << std::endl;
		unsigned int iter = 0;
		while(max_iter > iter) {
			iter++;
			double diff = 0.0;
			// copy vector x
			for( int i=0; i<rank(); i++) {
				x_old[i] = x[i];
			}
			std::cout << "Iter = " << iter;
			for( int j=0; j<rank(); j++) {
				d_ = b[j];
				int row_idx = this->row_ptr[j];
				int row_idx_next = this->row_ptr[j+1];
				for( int i=row_idx; i<row_idx_next; i++) {
					if( this->col_ind[i] == j ) continue; // skip aii
					if( this->col_ind[i] < j ) {
						d_ -= this->vals[i]*x[this->col_ind[i]];
					} else {
						d_ -= this->vals[i]*x_old[this->col_ind[i]];
					}
				}
				double t = d_/retrieve_element(j,j);
				diff += std::abs(x[j] - t);
				x[j] = t;
			}
			
			auto b_ = this->product_ax(x);
			auto diff_b = norm_vector_diff(b, b_);
			std::cout << ", difference of b = " << diff_b << std::endl;
			if( diff < epsilon ) {
				break;
			}
		}
		return x;
	}
	
	/// solve linear equation Ax=b using method[jacobi]
	/// iterate and get x
	/// \param b vector b, the size of b should equal to rank()
	/// \param epsilon when difference of x is less than epsilon, stop iteration
	/// \param max_iter when iteration number reach max_iter, stop iteratioin
	/// \param method iterate method
	/// \return root (x) of the linear equation
	std::vector<T> solve(const std::vector<T>& b, T epsilon,
	                     unsigned int max_iter = 4,
	                     solve_method method = jacobi, void* args = NULL) {
		switch( method ) {
			case jacobi:
				return solve_jacobi(b, epsilon, max_iter);
			case gauss_seidel:
				return solve_gauss_seidel(b, epsilon, max_iter);
			case sor:
				return solve_sor(b, epsilon, max_iter, args);
			default:
				std::cout << "Unknown Solver" << std::endl;
				return std::vector<T>();
		}
	}
	
	/// LOAD mtx file, will output one line of input every 2000 lines
	/// \param path
	/// \return Number of Non-Zero elements
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

	/// permute between row_i and row_j
	/// \param x
	/// \param row_i
	/// \param row_j
	/// \return 0
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

	/// Row_j += Row_i*a
	/// \param x
	/// \param row_i
	/// \param row_j
	/// \param a the scale
	/// \return 0
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

	/// output the matrix
	/// r: row_ptr
	/// c: col_ind
	/// v: vals
	/// Then will be the full matrix
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
	
	/// generate the test 5x5 matrix
	/// \return the test matrix
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
	
	/// generate the test matrix from raw input
	/// \param vals
	/// \param row_ptr
	/// \param col_idx
	/// \return the matrix for test
	static sparse_matrix generate_test_matrix(
			std::vector<T> vals,
			std::vector<T> row_ptr,
	    std::vector<T> col_idx
	) {
		sparse_matrix<T> m(row_ptr.size()-1);
		m.vals.clear();
		for (int i = 1; i <= vals.size(); i++) {
			m.vals.push_back((T)vals[i-1]);
		}
		m.row_ptr.clear();
		for( auto i: row_ptr) {
			m.row_ptr.push_back(i);
		}
		
		m.col_ind.clear();
		for( auto i: col_idx) {
			m.col_ind.push_back(i);
		}
		return m;
	}
};


#endif //ECE4960_SP18_MATRIX_H
