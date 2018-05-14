//
// Created by yj374 on 4/22/18.
//

#ifndef ECE4960_SP18_UTILS_H
#define ECE4960_SP18_UTILS_H

#include <cmath>

double i(double t) {
	double r = t - 20.0*std::floor(t/20.0);
	if( r < 1.0 ) {
		return r/10;
	} else if( r < 10.0 ) {
		return 0.1;
	} else if( r < 11 ) {
		return 0.1 - 0.1*(r-10);
	} else if( r < 20 ) {
		return 0.0;
	}
	return 0.0;
}

#endif //ECE4960_SP18_UTILS_H
