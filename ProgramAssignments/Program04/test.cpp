//
// Created by yj374 on 4/22/18.
//
#include <iostream>
#include "utils.h"

void test_i() {
	for (float t = 0.0; t < 100.0; t += 0.2) {
		std::cout << t << "\t" << i(t) << std::endl;
	}
}

int main(int argc, char** argv) {
	test_i();
	std::cout << i(20.0) << std::endl;
	return 0;
}
