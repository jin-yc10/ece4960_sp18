//
// Created by yj374 on 4/29/18.
//
#include <stdio.h>
#include <iostream>

double h = 1;
double delta_x = 1;
double delta_t = 1;
double v = 1;
double D = 1;
double f = 0;

/*
OUTPUT:
t=1 0.00000	0.00000	20.00000	-10.00000	0.00000	0.00000	0.00000
t=2 0.00000	0.00000	40.00000	-40.00000	10.00000	0.00000	0.00000
t=3 0.00000	0.00000	80.00000	-120.00000	60.00000	-10.00000	0.00000
t=4 0.00000	0.00000	160.00000	-320.00000	240.00000	-80.00000	0.00000
 */
int main(int argc, char** argv) {
	float x0[7] = {0, 0, 10, 0, 0, 0, 0};
	float x[7];
	for(int t=1; t<5; t++) {
		x[0] = 0;
		for(int i=1; i<6; i++) {
			x[i] = delta_t*v*(x0[i]-x0[i-1])+x0[i];
		}
		x[6] = 0;
		printf("t=%d ", t);
		for(int i=0; i<7; i++) {
			printf("%.5f\t",x[i]);
			x0[i] = x[i];
		}
		std::cout << std::endl;
	}
	return 0;
}