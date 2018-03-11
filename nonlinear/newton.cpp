//
// Created by 金宇超 on 3/11/18.
//

#include "newton.h"
#include <iostream>
#include <cmath>

double newton_raphson(equation f, equation df,
                    double init_x, double eps) {
	DEBUG_INFO(std::cout << "Run Newton Raphson" << std::endl);
	DEBUG_INFO(std::cout << " init_x = " << init_x << ", eps = " << eps << std::endl);
	double x = init_x;
	int step = 0;
	while(std::abs(f(x)) > eps) {
		step += 1;
		double dx = -1.0/df(x)*f(x);
		x += dx;
		DEBUG_INFO(std::cout << "step = " << step << ": " << x << " " << dx << ", f(x) = " << f(x) << std::endl);
	}
	return x;
}

double newton_raphson_line_search(equation f, equation df,
                                double init_x, double max_step, double eps) {
	DEBUG_INFO(std::cout << "Run Newton Raphson Line Search" << std::endl);
	DEBUG_INFO(std::cout << " init_x = " << init_x << ", eps = " << eps << std::endl);
	double x = init_x;
	int step = 0;
	while(std::abs(f(x)) > eps) {
		step += 1;
		double dx = -1.0/df(x)*f(x);
		double lb, ub, edge_x;
		if( dx > 0 ) {
			lb = x; ub = x+max_step; edge_x = x+max_step;
		} else {
			ub = x; lb = x-max_step; edge_x = x-max_step;
		}
		double out_x = 0.0;
		if( bisection_1d(f, lb, ub, out_x, eps, false) ) {
			x = out_x;
		} else {
			x = edge_x;
		}
		
		DEBUG_INFO(std::cout << "step = " << step << ": " << x << " " << dx << ", f(x) = " << f(x) << std::endl);
	}
	return x;
}