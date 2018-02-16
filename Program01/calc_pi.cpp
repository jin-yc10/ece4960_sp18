//
// Created by 金宇超 on 2/14/18.
//

#include <iostream>
#include <iomanip>
#include <cmath>
#include <math.h>
#include <stdio.h>
#include "program01.h"

using namespace std;

#define DIGITS 560
long b, c=DIGITS, d, e, f[DIGITS+1];

void calc_pi() {

	l2 << "\n===== calc_pi =====" << std::endl
	   << "1) 4tan(1)" << std::endl
     << "2) ln(640320^3+744)/sqrt(163)" << std::endl
	   << "3) taylor series of tan(1), 2sigma(n!/(2n+1)!!), each 14 term will get 4 digits" << std::endl
	   << "   in this program, we choose 560 terms in order to get 160(560/14*4) digits " << std::endl
		 << "   pi = 2 * (f[1]+1/3(f[2]+2/5(f[3]+...(f[560])...)) " << std::endl
	   << "   then we extract 4 digits out of the array and modify f[b] each time" << std::endl
	   << "   ref: https://cs.uwaterloo.ca/~alopez-o/math-faq/mathtext/node12.html" << std::endl;

  double pi;
  pi = 4*atan(1.0);
  l2 << "3."<< "1--------01--------01--------0" << std::endl;
	l2 << "pi = 4*atan(1.0)" << std::endl;
  l2 << setprecision(30) << pi << std::endl;
  double v = 640320.0*640320.0*640320.0+744.0;
  l2 << setprecision(30) << std::log(v)/std::sqrt(163.0) << std::endl;

	while(b-c!=0){
		f[b]=2000;
		b++;
	}

	l1 << "8. pi=";
	while(c!=0){
		b=DIGITS;
		d=f[b]*10000;
		f[b]=d%(b*2-1);
		d=d/(b*2-1);
		--b;
		while(b!=0){
			d=d*b+f[b]*10000;
			f[b]=d%(b*2-1);
			d=d/(b*2-1);
			--b;
		}
		l2 << setprecision(4) << e+d/10000;
		l1 << setprecision(4) << e+d/10000;
		c-=14;
		e=d%10000;
	}
	l1 << std::endl;
}