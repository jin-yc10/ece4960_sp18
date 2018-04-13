//
// Created by yj374 on 4/11/18.
//

/* OUTPUT
t x_true    k1        k2        k3        k4        x_rk4     e
0	2.000000	0.000000	0.000000	0.000000	0.000000	2.000000	0.000000
1	6.194600	3.000000	4.217299	3.912974	5.945677	6.201037	0.001039
2	14.843000	5.801645	8.729538	7.997565	12.712829	14.862484	0.001313
3	33.677000	12.380888	19.029761	17.367542	27.977693	33.721348	0.001317
4	75.339000	27.232032	42.109905	38.390437	62.074228	75.439172	0.001330
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
	double error;
};

std::vector<step> lk(double x0, double t0, double dt, double te) {
	std::vector<step> steps;
	double t_ = t0;
	double xi = x0, xi_1; // x_{i}, x_{i+1}
	step s0;
	s0.x = x0;
	steps.push_back(s0);
	while( t_ <= te ) {
		step s_;
		s_.k1 = func(t_, xi);
		s_.k2 = func(t_+0.5*dt, xi+0.5*(s_.k1*dt));
		s_.k3 = func(t_+0.5*dt, xi+0.5*(s_.k2*dt));
		s_.k4 = func(t_+dt, xi+(s_.k3*dt));
		xi_1 = xi+(s_.k1+2.0*s_.k2+2.0*s_.k3+s_.k4)*dt/6.0;
		s_.x = xi_1;
		steps.push_back(s_);
		xi = xi_1;
		t_ += dt;
	}
	return steps;
}

int main(int argc, char** argv) {
	double x0 = 2.000;
	std::vector<double> gt = {2.00, 6.1946, 14.843, 33.677, 75.339};
	auto steps = lk(x0, 0.0, 1.0, 4.0);
	//      1234567890123456789012345678901234567890123456789012345678901234567890
	printf("t   x_true      k1          k2          k3          k4          x_rk4       e\n");
	for( int i=0; i<5; i++) {
		printf("%d\t%06f\t%06f\t%06f\t%06f\t%06f\t%06f\t%06f\n",
		       i, gt[i],
		       steps[i].k1, steps[i].k2, steps[i].k3, steps[i].k4,
		       steps[i].x, (steps[i].x - gt[i])/gt[i]
		);
	}
	return 0;
}