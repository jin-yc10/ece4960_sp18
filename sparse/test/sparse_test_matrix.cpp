//
// Created by yj374 on 5/14/18.
//

#include "gtest/gtest.h"
#include "../matrix/sparse_matrix.h"

TEST(matrix, c1) {
	sparse_matrix<double> sp(6, 4);

	sp.add_element(1, 1, 5.0);
	sp.add_element(1, 4, 3.0);

	EXPECT_EQ(5.0, sp(1, 1));
	EXPECT_EQ(3.0, sp(1, 4));
	EXPECT_EQ(3.0, sp(1, 4));
}

TEST(matrix, c2) {
	sparse_matrix<double> sp = sparse_matrix<double>::generate_test_matrix();
	sp.add_element(1, 1, 0.0);
	sp.add_element(0, 0, 0.0);
	sp.report(false);
}

