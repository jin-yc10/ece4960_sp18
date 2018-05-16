//
// Created by yj374 on 5/14/18.
//

#include "common.h"
#include <iostream>

void test_vin() {
	for(double t=0; t<2.0; t += 0.2) {
		std::cout << t << "\t" << v_in(t) << std::endl;
	}
}

int main() {
	test_vin();
	return 0;
}