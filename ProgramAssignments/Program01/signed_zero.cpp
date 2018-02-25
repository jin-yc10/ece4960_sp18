//
// Created by yj374 on 2/14/2018.
//

#include <iostream>
#include <limits>
#include <cmath>
#include <utils/output.h>
#include "program01.h"

void signed_zero() {
	
	l2 << "\n=== signed_zero TEST ===\n"\
    "1/inf = zero, 1/ninf = n_zero\n"
	;
	
	float float_inf = std::numeric_limits<float>::infinity();
	float float_neg_inf = -float_inf;
	float zero = 1/float_inf;
	float neg_zero = 1/float_neg_inf;
	l2 << "zero " << float2hexstr(zero) << " " << zero << std::endl;
	l2 << "neg_zero " << float2hexstr(neg_zero) << " " << neg_zero << std::endl;
	l2 << "(zero == 0.0) = " << (zero == 0.0) << std::endl;
	l2 << "(zero == -0.0) = " << (zero == -0.0) << std::endl;
	l2 << "(zero == neg_zero) = " << (zero == neg_zero) << std::endl;
	l2 << "std::log(zero) = " << std::log(zero) << std::endl;
	l2 << "std::log(neg_zero) = " << std::log(neg_zero) << std::endl;
	l2 << "std::sin(zero)/zero = " << std::sin(zero)/zero << std::endl;
	l2 << "std::sin(neg_zero)/neg_zero = " << std::sin(neg_zero)/neg_zero << std::endl;
	l2 << "std::sin(neg_zero)/std::abs(neg_zero) = " << std::sin(neg_zero)/std::abs(neg_zero) << std::endl;
}