//
// Created by yj374 on 3/8/18.
//

#include <cmath>
#include <iostream>
#include <nonlinear/bisection.h>
#include <nonlinear/newton.h>

// Hacker Practice, Pg20, Pg25
// Newton Raphson, and Newton Raphson with Line Search

double f_(double x) {
	return std::exp(50*x)-1;
}

double df_(double x) {
	return 50*std::exp(50*x);
}

int main(int argc, char** argv) {
	newton_raphson(f_, df_, 1.0, 1e-8);
	newton_raphson(f_, df_, 10.0, 1e-8);
	newton_raphson_line_search(f_, df_, 1.0, 3.0, 1e-8);
	newton_raphson_line_search(f_, df_, 10.0, 3.0, 1e-8);
}