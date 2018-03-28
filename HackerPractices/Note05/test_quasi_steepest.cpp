//
// Created by yj374 on 3/26/18.
//

#include <iostream>
#include <stack>
#include <cmath>

double v(double x1, double x2) {
	double t1 = (3*x1*x1+x2-4);
	double t2 = (x1*x1-3*x2+2);
	return t1*t1+t2*t2;
}

//void cal_dv_close(double* d, double x1, double x2) {
//	d[0] = 40*x1*x1*x1-40*x1;
//	d[1] = 20*x2-20;
//}

void cal_dv(double* d, double x1, double x2) {
	double PT;
	if( x1 != 0.0 ) { PT = 1e-4*x1; }
	else { PT=1e-4; }
	d[0] = (v(x1+PT, x2)-v(x1,x2))/(PT);
	if( x2 != 0.0 ) { PT = 1e-4*x2; }
	else { PT=1e-4; }
	d[1] = (v(x1, x2+PT)-v(x1,x2))/(PT);
}

int main(int argc, char** argv) {
	double eps = 1e-8;
	double x[2] = {0.0, 0.0};
	double dx[2];
//	double dx_c[2];
	int max_step = 100;
	while(true) {
		max_step --;
		if( max_step == 0 ) {
			break;
		}
		double delv[2];
		double step = 0.012;
		double target_T = step;
		cal_dv(delv, x[0], x[1]);
		std::stack<std::pair<double, double>> candidates;
		candidates.push(std::make_pair(0.0, step));
		while( step > 1e-7 ) {
			double t = 0.0;
			double val_l = v(x[0]-t*delv[0], x[1]-t*delv[0]);
			t = step;
			double val_u = v(x[0]-t*delv[0], x[1]-t*delv[1]);
			if( val_u > val_l ) {
				step /= 2.0;
			} else {
				break;
			}
		}
		dx[0] = -target_T * delv[0];
		dx[1] = -target_T * delv[1];
		x[0] += dx[0];
		x[1] += dx[1];
		double diff = dx[0]*dx[0]+dx[1]*dx[1];
		double val = v(x[0], x[1]);
		std::cout << x[0]*x[0]+x[1]*x[1] << "\t" << diff << "\t" << target_T << "\t" << val << std::endl;
		if( diff < eps ) {
			break;
		}
	}
	return 0;
}
