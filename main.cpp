//
// Created by yj374 on 1/30/18.
//

#include <iostream>
#include <cmath>
#include <limits>
#include "utils/output.h"



std::string double2hexstr(double x) {
	union
	{
		long long i;
		double    d;
	} value;
	
	value.d = x;
	char buf[17];
	snprintf (buf,sizeof(buf),"%016llx",value.i);
	buf[16]=0; //make sure it is null terminated.
	return std::string(buf);
}

union val {int i; float f;};

bool isPositiveZero(float x) {
	val _x;
	_x.f = x;
	return _x.i==0x00000000;
}
bool isNegativeZero(float x) {
	val _x;
	_x.f = x;
	return _x.i==0x80000000;
}

union val_db {
	int i[2];
	double d;
};

int main(int argc, char** argv) {
//	char v[8];
//	unsigned int x = 0xffffffff;
//	unsigned long lx = 0xffffffffffffffff;
//	*v = lx;
//	double dx = *(double*)v;
//	std::cout << x << std::endl;
//	std::cout << lx << std::endl;
//	std::cout << dx << std::endl;
	
//	int m = 1;
//	unsigned int um = 1;
//	unsigned long ul = 1;
//	numeric f = 1.0f;
//	double d = 1.0;
//	for( int i=1; i<=101; i++) {
//		m*=i;
//		um*=i;
//		ul*=i;
//		f*=i;
//		d*=i;
//	}
//	std::cout << m << std::endl;
//	std::cout << (unsigned int)m << std::endl;
//	std::cout << std::isfinite(um) << std::endl;
//	std::cout << ul << std::endl;
//	std::cout << f << "\t" << std::isfinite(f) << std::endl;
//	std::cout << d << std::endl;
//
//	double a = 1.0e23;
//	double b = 1.0e23-1;
//	std::cout << a-b << std::endl;
//	std::cout << double2hexstr(a) << " : " << double2hexstr(b) << std::endl;
//
//	double x = 0.0, y = 0.0, doubleResult1, doubleResult2;
//	doubleResult1 = 1/x;
//	doubleResult2 = y/x;
//	printf("%f,%f\n", doubleResult1, doubleResult2);
	
// Observe NaN and INF handling in integers
//
//	long m = 0, n = 0, intResult1, intResult2;
//	intResult1 = 1/m;
//	intResult2 = m/n;
//	printf("%d,%d\n",intResult1, intResult2);
	
//	val v;
//	v.f = 0.0f;
//	std::cout << v.f << std::endl;
//	std::cout << isPositiveZero(v.f) << std::endl;
//	std::cout << isNegativeZero(v.f) << std::endl;
//
//	v.i = 0x80000000;
//	std::cout << v.f << std::endl;
//	std::cout << isPositiveZero(v.f) << std::endl;
//	std::cout << isNegativeZero(v.f) << std::endl;
//
	val_db vd;
	vd.i[1] = 0x00000000;
	vd.i[0] = 0x00000001;
	std::cout << vd.d << "\t" << double2hexstr(vd.d) << std::endl;
	
	vd.d = 1.0;
	std::cout << vd.d << "\t" << double2hexstr(vd.d) << std::endl;
//	std::cout << std::numeric_limits<double>::denorm_min() << std::endl;
	
	{
		double x = 1.234567890123456;
		int i = 1;
	
		// The normalized number is above 4.9407*10^(-324)
		x *= 1e-307;
	
		// Decrease the normalized number to the range of denormals
		for (i=1; i<20; i++) {
			x /= 10.0;
			printf("%0.18e, %s\n",(x), double2hexstr(x).c_str());
		}
	}
	
	return 0;
}