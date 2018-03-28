//
// Created by yj374 on 3/25/18.
//

#include <nonlinear/bisection.h>
#include <iostream>

#define PT (1e-4)

double v(double x1, double x2) {
	double t1 = (3*x1*x1+x2-4);
	double t2 = (x1*x1-3*x2+2);
	return t1*t1+t2*t2;
}

void cal_dv(double* d, double x1, double x2) {
	d[0] = (v(x1+PT, x2)-v(x1-PT,x2))/(2*PT);
	d[1] = (v(x1, x2+PT)-v(x1,x2-PT))/(2*PT);
}

void cal_hv(double* h, double x1, double x2) {
	h[0] = (v(x1+PT, x2)-2*v(x1, x2) + v(x1-PT, x2))/(PT*PT);
	h[1] = (v(x1+PT, x2+PT) - v(x1+PT, x2-PT) - v(x1-PT, x2+PT) + v(x1-PT, x2-PT))/(4.0*PT*PT);
	h[2] = h[1];
	h[3] = (v(x1, x2+PT)-2*v(x1, x2) + v(x1, x2-PT))/(PT*PT);
}

void cal_invh(double* invh, double* h) {
	double a = h[0], b = h[1], c = h[2], d = h[3];
	double det = a*d-b*c;
	invh[0] = d/det;
	invh[1] = -b/det;
	invh[2] = -c/det;
	invh[3] = a/det;
}

void cal_dx(double* dx_, double* invh, double* dv) {
	dx_[0] = -(invh[0]*dv[0]+invh[1]*dv[1]);
	dx_[1] = -(invh[2]*dv[0]+invh[3]*dv[1]);
}

int main(int argc, char** argv) {
	double x[2] = {0.4, -10.0};
	double dx[2];
	double delv[2];
	double Hx[4];
	double invH[4];
	double eps = 1e-8;
	while(true) {
		cal_dv(delv, x[0], x[1]);
		std::cout << "delv = ";
		for(int i=0; i<2; i++) {
			std::cout << delv[i] << " ";
		}
		std::cout << std::endl;
		cal_hv(Hx, x[0], x[1]);
		std::cout << "h = ";
		for(int i=0; i<4; i++) {
				std::cout << Hx[i] << " ";
		}
		std::cout << "\n";
		std::cout << "invh = ";
		cal_invh(invH, Hx);
		for(int i=0; i<4; i++) {
			std::cout << invH[i] << " ";
		}
		std::cout << "\n";
		cal_dx(dx, invH, delv);
		std::cout << "dx = ";
		for(int i=0; i<2; i++) {
			std::cout << dx[i] << " ";
			x[i] += dx[i];
		}
		std::cout << "\n";
		std::cout << "x = ";
		for(int i=0; i<2; i++) {
			std::cout << x[i] << " ";
		}
		std::cout << "\n";
		double diff = dx[0]*dx[0]+dx[1]*dx[1];
		double val = v(x[0], x[1]);
		std::cout << "V = " << val << std::endl << std::endl;
		if( diff < eps ) {
			break;
		}
	}
	return 0;
}