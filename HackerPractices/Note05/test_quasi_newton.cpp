//
// Created by yj374 on 3/25/18.
//

#include <cmath>
#include <iostream>
#include <nonlinear/bisection.h>
#include <nonlinear/newton.h>

// Hacker Practice, Pg30
// Quasi-Newton method

double f_(double x) {
	return std::exp(100*x)-1;
}

double df_(double x) {
	return 100*std::exp(100*x);
}

double quasi(double x) {
	return (0.0001*x)/(f_(1.0001*x)-f_(x));
}

int main(int argc, char** argv) {
	double eps = 1e-8;
	double init_x = 1.0;
	double x = init_x;
	int step = 0;
	while(std::abs(f_(x)) > eps) {
		step += 1;
		double dx = -quasi(x)*f_(x);
		x += dx;
		DEBUG_INFO(std::cout << "step = " << step << ": x=" << x << " dx=" << dx << ", f(x) = " << f_(x) << std::endl);
	}
	
	newton_raphson(f_, df_, 1.0, 1e-8);
	
	return 0;
}