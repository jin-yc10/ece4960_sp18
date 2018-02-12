//
// Created by 金宇超 on 2/6/18.
//

#include <iostream>

inline double f1(double x) {
	return x*x;
}

inline double f2(double x) {
	return x*x+1.0e8;
}

inline double f3(double x) {
	return x*x*x;
}

#define f f3

inline double Ah(double x, double h) {
	return (f(x+h)-f(x))/h;
}

int main(int argc, char** argv) {
	double x=1.0;
	double h=1.0e-4;
	double f_ = 0.0;
	double f_2 = 0.0;
	double f_3 = 0.0;
	double real = 3.0;
	double e2h, eh, a4h, a2h, ah;
	for(int i=0; i<36; i++) {
		f_ = (f(x+h)-f(x))/h;
		f_2 = (f(x+2*h)-f(x))/(2*h);
		f_3 = -1/(2*h)*f(x+2*h)-3/(2*h)*f(x)+2/h*f(x+h);
		e2h = f_2 - real;
		eh = f_ - real;
		double term = (Ah(x, 4*h) - Ah(x, 2*h))/(Ah(x, 2*h) - Ah(x, h));
		printf("%.06e\t%.06e\t%.06e\t%.06e\t%.06e\t%.06e\n", h, f_-real, f_2-real, f_3-real, e2h/eh, term);
		h /= 10.0f;
	}
}