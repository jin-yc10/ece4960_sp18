//
// Created by 金宇超 on 3/7/18.
//

#include <matrix/test_matrix.h>

int main(int argc, char** argv) {
	matrix_test_suite t;
	t.test_sparse_matrix_init();
	t.test_add_element();
	t.test_retrieve_element();
	t.test_product_ax();
	t.test_row_permute();
	t.test_row_scale();
	return 0;
}
