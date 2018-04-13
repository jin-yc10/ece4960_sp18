//
// Created by yj374 on 4/11/18.
//

/* OUTPUT
t x_true    x_rk4     e         error     h
0	2.000000	2.000000	0.000000	0.000000	0.000000
1	6.194600	6.201037	0.001039	-0.389505	0.542272
2	14.843000	14.862484	0.001313	-0.907873	0.547167
3	33.677000	33.721348	0.001317	-2.045395	0.548385
4	75.339000	75.439172	0.001330	-4.567210	0.548699
 */

#include <cmath>
#include <iostream>
#include <vector>

double func(double t, double x) {
	return 4*std::exp(0.8*t) - 0.5*x;
}

struct step {
	double k1;
	double k2;
	double k3;
	double k4;
	double x;
	double E;
	double h_error;
};

std::vector<step> lk4(double x0, double t0, double dt, double te) {
	std::vector<step> steps;
	double t_ = t0;
	double xi = x0, xi_1, xi_1_rk3; // x_{i}, x_{i+1}
	step s0;
	s0.x = x0;
	steps.push_back(s0);
	double er = 1e-2, ea=1e-2;
	while( t_ <= te ) {
		step s_;
		s_.k1 = func(t_, xi);
		s_.k2 = func(t_+0.5*dt, xi+0.5*(s_.k1*dt));
		s_.k3 = func(t_+0.5*dt, xi+0.5*(s_.k2*dt));
		s_.k4 = func(t_+dt, xi+(s_.k3*dt));
		xi_1 = xi+(s_.k1+2.0*s_.k2+2.0*s_.k3+s_.k4)*dt/6.0;
		xi_1_rk3 = xi+(2.0*s_.k1+3.0*s_.k2+4.0*s_.k3)*dt/9.0;
		s_.x = xi_1;
		s_.E = xi_1_rk3 - xi_1;
		s_.h_error = std::pow(er/(std::abs(s_.E)/(std::abs(xi_1)+ea)), 0.3333333);
		steps.push_back(s_);
		xi = xi_1;
		t_ += dt;
	}
	return steps;
}

int main(int argc, char** argv) {
	double x0 = 2.000;
	std::vector<double> gt = {2.00, 6.1946, 14.843, 33.677, 75.339};
	auto steps = lk4(x0, 0.0, 1.0, 4.0);
	//      1234567890123456789012345678901234567890123456789012345678901234567890
	printf("t   x_true      x_rk4       e           error       h\n");
	for( int i=0; i<5; i++) {
		printf("%d\t%06f\t%06f\t%06f\t%06f\t%06f\n",
		       i, gt[i],
		       steps[i].x, (steps[i].x - gt[i])/gt[i], steps[i].E, steps[i].h_error
		);
	}
	return 0;
}