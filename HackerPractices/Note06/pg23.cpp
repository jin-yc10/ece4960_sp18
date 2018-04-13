//
// Created by yj374 on 4/11/18.
//

#include <iostream>
#include <vector>
#include <cmath>

double func(double t, double x) {
	return 4*std::exp(0.8*t) - 0.5*x;
}

std::vector<double> forward_f(double f0, double dt, double t_start, double t_end) {
	std::vector<double> fs;
	double f = f0;
	double t = t_start;
	while(t <= t_end) {
		fs.push_back(f);
		f = f + func(t, f)*dt;
		t += dt;
	}
	return fs;
}

std::vector<double> huen(double x0, double dt, double t_start, double t_end, bool is_onestep) {
	std::vector<double> xs;
	double x_i = x0;
	double x_i1;
	double t_i = t_start; // t_{i}
	double t_i1; // t_{i+1}
	double tol = 1e-7;
	while(t_i <= t_end) {
		xs.push_back(x_i);
		if( is_onestep ) {
			double x_i1_j0 = x_i + func(t_i, x_i)*dt; // predictor
			double x_i1_j1 = x_i + 0.5*(func(t_i, x_i)+func(t_i+dt, x_i1_j0))*dt; // corrector
			x_i = x_i1_j1;
		} else {
			double x_i1_j = x_i + func(t_i, x_i)*dt; // predictor
			double x_i1_j1;
			double error;
			while( true ) {
				x_i1_j1 = x_i + 0.5*(func(t_i, x_i)+func(t_i+dt, x_i1_j))*dt; // corrector
				error = std::abs((x_i1_j1-x_i1_j)/x_i1_j1);
				if( error < tol) {
					break;
				}
				x_i1_j = x_i1_j1;
			}
			x_i = x_i1_j1;
		}
		t_i += dt;
	}
	return xs;
}

double er(double g, double v) {
	return 100.0*(v-g)/g;
}

int main(int argc, char** argv) {
	double f0 = 2.000;
	double dt = 1.000;
	
	std::vector<double> gt = {2.00, 6.1946, 14.843, 33.677, 75.339};
	auto forward_euler = forward_f(f0, dt, 0.0, 4.0);
	auto one_step_huen = huen(f0, dt, 0.0, 4.0, true);
	auto iter_huen = huen(f0, dt, 0.0, 4.0, false);
	
	printf("t   x_true      forward     e           one         e           iter        e      \n");
	for( int i=0; i<gt.size(); i++) {
		printf("%d\t%06f\t%06f\t%06f\t%06f\t%06f\t%06f\t%06f\n",
		       i, gt[i], forward_euler[i], er(gt[i], forward_euler[i]),
		       one_step_huen[i], er(gt[i], one_step_huen[i]),
		       iter_huen[i], er(gt[i], iter_huen[i])
		);
	}
	return 0;
}