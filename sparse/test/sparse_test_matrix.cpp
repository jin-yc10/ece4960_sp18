//
// Created by yj374 on 5/14/18.
//

#include "gtest/gtest.h"
#include "../matrix/sparse_matrix.h"

TEST(test2, c1) {
	sparse_matrix<double> sp(5);
	sp.add_element(1,1,5.0);
	EXPECT_EQ(5.0, sp.retrieve_element(1, 1));
	EXPECT_NEAR(4.999, sp.retrieve_element(1, 1), 0.1);
}

