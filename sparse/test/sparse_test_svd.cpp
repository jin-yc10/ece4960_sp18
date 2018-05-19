#include "gtest/gtest.h"
#include "../matrix/sparse_matrix.h"
#include "../svd/sparse_svd.h"

TEST(SVD, c1) {
	sparse_matrix<double> sp(5, 4);
	sp.add_element(0, 0, 1.0);
	sp.add_element(0, 2, 2.0);
	sp.add_element(1, 3, 4.0);
	sp.add_element(2, 1, 3.0);
	sp.add_element(3, 2, 5.0);
	sp.add_element(4, 0, 2.0);
	sp.add_element(4, 3, 2.0);  
	sparse::svd svd(&sp);
	svd.calc();
	svd.J->report(false);
	for (auto e : svd.Es) {
		std::cout << e << " ";
	}
	std::cout << std::endl;
// Supposed to be
// [5.40169348 4.57946293 3.         1.96219949]
	EXPECT_NEAR(svd.Es[0], 5.40169348, 1e-5);
	EXPECT_NEAR(svd.Es[1], 4.57946293, 1e-5);
	EXPECT_NEAR(svd.Es[2], 3.0, 1e-5);
	EXPECT_NEAR(svd.Es[3], 1.96219949, 1e-5);
}