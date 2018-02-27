//
// Created by 金宇超 on 2/27/18.
//

#include <iostream>

/*
Output on Mac, Clang:
e = 0.001, d = 0
e = 0.0001, d = 2.02954e-19
e = 1e-05, d = 2.68521e-18
e = 1e-06, d = 3.32508e-13
e = 1e-07, d = 3.49643e-11
e = 1e-08, d = 7.20525e-12
e = 1e-09, d = 6.31145e-07
e = 1e-10, d = 3.262e-05
e = 1e-11, d = 0.000893266
e = 1e-12, d = 0.0019332
e = 1e-13, d = 24.2568
e = 1e-14, d = 1070.37
e = 1e-15, d = nan
*/

int main(int argc, char** argv) {
	double e = 1e-2;
	double A[4] = {100,99,99,98.01};
	double A_inv[4];
	for(int i=2; i<15; i++) {
		A[3] = 98.01 - e;
		double det = A[0]*A[3]-A[1]*A[2];
		A_inv[0] = A[3]/det;
		A_inv[1] = -A[2]/det;
		A_inv[2] = -A[1]/det;
		A_inv[3] = A[0]/det;
		e /= 10.0;
		double x = 199*A_inv[0]+197*A_inv[1];
		double y = 199*A_inv[2]+197*A_inv[3];
		double dx = A[0]*x+A[1]*y;
		double dy = A[2]*x+A[3]*y;
		double d = (dx-199)*(dx-199)+(dy-197)*(dy-197);
		std::cout << "e = " << e << ", d = " << d << std::endl;
	}
}