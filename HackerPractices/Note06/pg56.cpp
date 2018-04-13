//
// Created by yj374 on 4/11/18.
//

/* OUTPUT
t x_true    x_tr      absolute  error     h
0	2.000000	2.000000	0.000000	0.000000	0.000000
1	6.194600	7.058236	0.863636	-0.526865	0.011024
2	14.843000	18.238205	3.395205	-1.063929	0.011968
3	33.677000	43.044637	9.367637	-2.262409	0.012391
4	75.339000	98.179544	22.840544	-4.932802	0.012579
*/

#include <cmath>
#include <iostream>
#include <vector>

double func(double t, double x) {
	return 4*std::exp(0.8*t) - 0.5*x;
}

double func_dx(double t, double x) {
	return -0.5;
}

double func_dt(double t, double x) {
	return 3.2*std::exp(0.8*t);
}

double func_gt(double t) {
	return (std::exp(0.8*t)-std::exp(-0.5*t))*4.0/1.3+2.0*std::exp(-0.5*t);
}

struct step {
	double x;
	double xg;
	double xg_gt;
	double E;
	double h_error;
};

std::vector<step> tr_bdf2(double x0, double t0, double dt, double te) {
	std::vector<step> steps;
	double gm = 2-std::sqrt(2.0);
	double arg1 = ((1-gm)*(1-gm))/(gm*(2-gm)); // (1-r)2 / r(2-r)
	double arg2 = 1.0/(gm*(2-gm)); // 1 / r(2-r)
	double arg3 = (1-gm)/(2-gm);
	double t_ = t0;
	double xi = x0;
	step s0;
	s0.x = x0;
	steps.push_back(s0);
	double er = 1e-7, ea=1e-7;
	while( t_ <= te ) {
		step s_;
		// t+gamma*dt
		double f_i = func(t_, xi);
		double f_i1 = func(t_+dt, xi);
		double x_ig0 = xi + f_i*gm*dt; // predictor
		double x_ig = xi + 0.5*(f_i+func(t_+gm*dt, x_ig0))*gm*dt; // corrector
		double f_g = func(t_+gm*dt, x_ig);
		
		// newton solver
		double nt_x = xi;
		while( true ) {
			double f = arg1*xi+arg2*x_ig+arg3*dt*func(t_+dt, nt_x) - nt_x;
			double df = -0.5 - 1;
			double nt_dx = -f/df;
			nt_x += nt_dx;
			if( std::abs(nt_dx) < 1e-9 ) {
				break;
			}
		}
		double xi1 = nt_x;
		s_.x = xi1;
		s_.xg = x_ig;
		s_.xg_gt = func_gt(t_+gm*dt);
		s_.E = (f_i/gm-f_g/(gm*(1-gm))+f_i1/(1-gm))*(3*gm*gm-4*gm+2)/(6.0*(gm-2));
		s_.h_error = std::pow(er/(std::abs(s_.E)/(std::abs(xi1)+ea)), 0.3333333);
		steps.push_back(s_);
		xi = xi1;
		t_ += dt;
	}
	return steps;
}

int main(int argc, char** argv) {
	double x0 = 2.000;
	std::vector<double> gt = {2.00, 6.1946, 14.843, 33.677, 75.339};
	auto steps = tr_bdf2(x0, 0.0, 1.0, 4.0);
	//      1234567890123456789012345678901234567890123456789012345678901234567890
	printf("t   x_true      x_tr        absolute    error       h\n");
	for( int i=0; i<5; i++) {
		printf("%d\t%06f\t%06f\t%06f\t%06f\t%06f\n",
		       i, gt[i],
		       steps[i].x, (steps[i].x - gt[i]), steps[i].E, steps[i].h_error
		);
	}
	return 0;
}