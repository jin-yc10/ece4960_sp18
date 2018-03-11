//
// Created by yj374 on 3/8/18.
//

// Hacker Practice, Note05, Pg05, Bisection Search

#include <nonlinear/bisection.h>
#include <cmath>
#include <iostream>

double f(double x) {
	return std::exp(x)-1;
}

double f1(double x, double y) {
	return std::exp(x)-std::exp(y)-0.0;
}

double f2(double x, double y) {
	return std::exp(x)+std::exp(y)-2.0;
}

extern

int main(int argc, char** argv) {
	
	double ret1;
	bisection_1d(f, -5.0, 10.0, ret1, 1e-7, true);
	std::cout << "bisection_1d, x = " << ret1 << ", residual f(x) = "<< f(ret1) << std::endl;
	
	range init_x; init_x.lb = -5.0; init_x.ub = 10.0;
	range init_y; init_y.lb = -5.0; init_y.ub = 10.0;
	auto ret = bisection_2d(init_x, init_y, f1, f2, 1e-10);
	std::cout << "bisection_2d, x = " << ret.first << ", y = " << ret.second << std::endl;
}