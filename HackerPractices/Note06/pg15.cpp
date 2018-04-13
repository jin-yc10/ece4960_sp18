//
// Created by yj374 on 4/10/18.
//

#include <cmath>
#include <iostream>
#include <vector>

std::vector<double> gt_f(double f0, double dt, double t_start, double t_end) {
	std::vector<double> fs;
	double f = f0;
	double t = t_start;
	while(t <= t_end) {
		fs.push_back(std::exp(-t));
		t += dt;
	}
	return fs;
}

std::vector<double> backward_f(double f0, double dt, double t_start, double t_end) {
	std::vector<double> fs;
	double f = f0;
	double t = t_start;
	while(t <= t_end) {
		fs.push_back(f);
		f = f/(1+dt);
		t += dt;
	}
	return fs;
}

std::vector<double> trapezoidal_f(double f0, double dt, double t_start, double t_end) {
	std::vector<double> fs;
	double f = f0;
	double t = t_start;
	while(t <= t_end) {
		fs.push_back(f);
		f = f*(2-dt)/(2+dt);
		t += dt;
	}
	return fs;
}

void test_case(double dt) {
	double f0 = 1.0;
	auto gt = gt_f(f0, dt, 0.0, 20.0);
	auto bf = backward_f(f0, dt, 0.0, 20.0);
	auto tf = trapezoidal_f(f0, dt, 0.0, 20.0);
	std::cout << " =============== " << std::endl;
	std::cout << "dt = " << dt << std::endl;
	for( int i=0; i<gt.size(); i++) {
		std::cout << gt[i] << "\t" << bf[i] << "\t" << tf[i] << std::endl;
	}
}

int main(int argc, char** argv) {
	test_case(0.5);
	test_case(1.0);
	return 0;
}