//
// Created by 金宇超 on 2/14/18.
//

#ifndef ECE4960_SP18_FLOATS_H
#define ECE4960_SP18_FLOATS_H

union float_union {
	float f;
	int i;
};

struct float_ieee754 {
	int sign;
	int exponent;
	int mantissa;
};

bool is_inf(float f);
bool is_ninf(float f);

#endif //ECE4960_SP18_FLOATS_H
