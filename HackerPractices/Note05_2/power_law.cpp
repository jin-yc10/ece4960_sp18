//
// Created by yj374 on 3/28/18.
//

#include <cmath>
#include <iostream>

double V(double a, double m, double* x, double* y, int N) {
	double sum = 0.0;
	for(int i=0; i<N; i++) {
		double t = (a*std::pow(x[i], m) - y[i]);
		sum += t*t;
	}
	return sum;
}

double f1(double a, double m, double* x, double* y, int N) {
	double sum = 0.0;
	for(int i=0; i<N; i++) {
		double t = (a*std::pow(x[i], 2.0*m) - std::pow(x[i], m)*y[i]);
		sum += t;
	}
	return sum;
}

double f2(double a, double m, double* x, double* y, int N) {
	double sum = 0.0;
	for(int i=0; i<N; i++) {
		double t = (a*a*std::log(x[i])*std::pow(x[i], 2.0*m) - a*std::pow(x[i], m)*std::log(x[i])*y[i]);
		sum += t;
	}
	return sum;
}

void cal_invh(double* invh, double* h) {
	double a = h[0], b = h[1], c = h[2], d = h[3];
	double det = a*d-b*c;
	invh[0] = d/det;
	invh[1] = -b/det;
	invh[2] = -c/det;
	invh[3] = a/det;
}

int main(int argc, char** argv) {
	int N = 6;
	double x[6] = {1.0, 4.5, 9.0, 20, 74, 181};
	double y[6] = {3.0, 49.4, 245, 1808, 2.2e4, 7.3e4};
	double a = 2, m = 1;
	double da, dm;
	double diff_a, diff_m;
	double h[4];
	double invh[4];
	double pret = 1e-10;
	int max_step = 100;
	while(max_step > 0) {
		max_step --;
		h[0] = 0.0;
		for(int i=0; i<N; i++) {
			h[0] += std::pow(x[i], 2.0*m);
		}
		h[1] = (f1(a, m+pret*m, x, y, N) - f1(a, m, x, y, N)) / (pret*m);
		h[2] = 0.0;
		for(int i=0; i<N; i++) {
			h[2] += (2.0*a*std::log(x[i])*std::pow(x[i], 2.0*m) -
					std::log(x[i])*std::pow(x[i], m)*y[i]);
		}
		h[3] = (f2(a, m+pret*m, x, y, N) - f2(a, m, x, y, N)) / (pret*m);
		std::cout << h[0] << "\t" << h[1] << "\t" << h[2] << "\t" << h[3] << std::endl;
		cal_invh(invh, h);
		double da2 = f1(a, m, x, y, N);
		double dm2 = f2(a, m, x, y, N);
		diff_a = -(invh[0]*da2+invh[1]*dm2);
		diff_m = -(invh[2]*da2+invh[3]*dm2);
		a += diff_a;
		m += diff_m;
		std::cout << "V=" << V(a, m, x, y, N);
		std::cout << "\ta=" << a << "," << "m=" << m << std::endl;
		if( diff_a*diff_a+diff_m*diff_m < 1e-7 ) {
			break;
		}
	}
	std::cout << "V=" << V(a, m, x, y, N) << std::endl;
	return 0;
}