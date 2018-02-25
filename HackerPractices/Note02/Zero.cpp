//
// Created by yj374 on 2/4/2018.
//

#include <iostream>
#include <cmath>

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

int main(int argc, char** argv) {
	val v;
	v.f = 0.0f;
	std::cout << v.f << std::endl;
	std::cout << isPositiveZero(v.f) << std::endl;
	std::cout << isNegativeZero(v.f) << std::endl;

	v.i = 0x80000000;
	std::cout << v.f << std::endl;
	std::cout << isPositiveZero(v.f) << std::endl;
	std::cout << isNegativeZero(v.f) << std::endl;
}