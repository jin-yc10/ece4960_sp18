//
// Created by 金宇超 on 2/15/18.
//

#ifndef ECE4960_SP18_MATRIX_H
#define ECE4960_SP18_MATRIX_H

#include <vector>
#include <stdio.h>

template<class T>
class sparse_matrix {
private:
	std::vector<T> vals;
	std::vector<int> row_ptr;
	std::vector<int> col_ind;
public:
	sparse_matrix(int rank) {
	
	}
	
	void add_element(int rowId, int colId, T val) {
		if( val == 0.0 ) { // TODO: this comparision might have problem
			return; // add a zero, directly jump out
		}
		int row_idx = row_ptr[rowId];
		int col_idx = -1;
		int i=row_idx;
		for( i=row_idx; i<row_ptr[rowId+1]; i++) {
			if( col_ind[i] == colId ) {
				vals[i] = val; // already exist, replace it
				return;
			}
		}
		vals.insert(vals.begin()+i, val);
		col_ind.insert(col_ind.begin()+i, colId);
		for( int r=rowId+1; r<this->rank(); r++) {
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
		return 0;
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
		m.row_ptr.push_back(0);
		m.row_ptr.push_back(3);
		m.row_ptr.push_back(6);
		m.row_ptr.push_back(9);
		m.row_ptr.push_back(10);
		m.row_ptr.push_back(12);
		
		int col_raw[] = {0, 1, 4,
		                 0, 1, 2,
		                 1, 2, 4,
		                 4,
		                 0, 4};
		m.col_ind = std::vector<int>(col_raw, col_raw + sizeof(col_raw) / sizeof(col_raw[0]) );
		return m;
	}
};


#endif //ECE4960_SP18_MATRIX_H
