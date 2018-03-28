//
// Created by yj374 on 3/28/18.
//

#include <random>
#include <iostream>

std::random_device rd;
std::mt19937 mt(rd());
std::uniform_real_distribution<double> dist(0.0, 1.0);

int main(int argc, char** argv) {
	double lambda = 0.2;
	double vs[1000];
	double max_v = 0.0, min_v = 1e20;
	for( int i=0; i<1000; i++ ){
		double u = dist(mt);
		double v = -std::log(1-u)/lambda;
		vs[i] = v;
		if(v < min_v) min_v = v;
		if(v > max_v) max_v = v;
	}
	std::cout << "v=[" << min_v << "," << max_v << "]" << std::endl;
	int bins[20] = {0};
	for( int i=0; i<1000; i++) {
		int idx = (int)(vs[i]/0.5);
		if( idx >= 20 ) idx = 19;
		bins[idx] += 1;
	}
	for( int i=0; i<20; i++) {
		std::cout << bins[i] << std::endl;
	}
	return 0;
}
