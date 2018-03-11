//
// Created by 金宇超 on 3/11/18.
//

#include "bisection.h"
#include <cmath>
#include <iostream>
#include <vector>
#include <stack>

bool bisection_1d(equation f, double lb_, double ub_, double& out,
                  double eps_, bool output_iteration) {
	double eps = eps_;
	double lb = lb_;
	double ub = ub_;
	int step = 0;
	while( std::abs(f((lb+ub)/2.0)) > eps ) {
		step += 1;
		double mid = (lb+ub)/2.0;
		if( output_iteration )
			std::cout << "Step: " << step << ", " << f((lb+ub)/2.0) << std::endl;
		double fl = f(lb);
		double fu = f(ub);
		double fc = f(mid);
		
		if( fl*fu > 0 ) {
			// both are positive or negative
			DEBUG_INFO(std::cout << "No result in range" << std::endl);
			return false;
		}
		
		if( fl*fc < 0 ) {
			ub = mid;
			continue;
		}
		if( fu*fc < 0 ) {
			lb = mid;
			continue;
		}
	}
//	std::cout << f((lb+ub)/2.0) << std::endl;
	out = (lb+ub)/2.0;
	return true;
}

bool is_sign_change(equation_2d f, range rx, range ry) {
	int positive = 0;
	int negative = 0;
	double f1s[4] = {f(rx.lb, ry.lb), f(rx.lb, ry.ub), f(rx.ub, ry.lb), f(rx.ub, ry.ub)};
	for( int i=0; i<4; i++) {
		if( f1s[i] > 0.0 ) {
			positive += 1;
		} else {
			negative += 1;
		}
	}
	return (positive > 0 && negative > 0);
}

double mid_value(range v) {
	return (v.ub+v.lb)/2.0;
}

std::stack<std::pair<range, range>> candidates;

std::pair<double, double> bisection_2d(range init_rx, range init_ry,
                                       equation_2d f1, equation_2d f2, double eps) {
	candidates.push(std::make_pair(init_rx, init_ry));
	while( !candidates.empty() ) {
		std::pair<range, range> t;
		t = candidates.top();
		range rx = t.first;
		range ry = t.second;
		double mid_x = mid_value(rx); double mid_y = mid_value(ry);
		if( std::abs(f1(mid_x, mid_y)) < eps && std::abs(f2(mid_x, mid_y)) < eps ) {
//			std::cout << "Got! " << mid_x << " " << mid_y << " " << f1(mid_x, mid_y) << " " << f2(mid_x, mid_y) << std::endl;
			return std::make_pair(mid_x, mid_y);
		} else {
//			std::cout << "Try! " << mid_x << " " << mid_y << " " << f1(mid_x, mid_y) << " " << f2(mid_x, mid_y) << std::endl;
		}
		candidates.pop();
		if( mid_x - rx.lb < 1e-15 || mid_y - ry.lb < 1e-15 ) {
			continue;
		}
		if( is_sign_change(f1, t.first, t.second) && is_sign_change(f2, t.first, t.second)) {
			range rx_, ry_;
			rx_ = rx; ry_ = ry; rx_.ub = mid_x; ry_.ub = mid_y; candidates.push(std::make_pair(rx_, ry_));
			rx_ = rx; ry_ = ry; rx_.lb = mid_x; ry_.ub = mid_y; candidates.push(std::make_pair(rx_, ry_));
			rx_ = rx; ry_ = ry; rx_.ub = mid_x; ry_.lb = mid_y; candidates.push(std::make_pair(rx_, ry_));
			rx_ = rx; ry_ = ry; rx_.lb = mid_x; ry_.lb = mid_y; candidates.push(std::make_pair(rx_, ry_));
		} else {
			continue;
		}
	}
	return std::make_pair(std::numeric_limits<double>::quiet_NaN(),
	                      std::numeric_limits<double>::quiet_NaN());
}