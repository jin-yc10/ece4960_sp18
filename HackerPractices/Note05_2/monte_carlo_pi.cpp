//
// Created by yj374 on 3/28/18.
//

#include <random>
#include <iostream>

int main(int argc, char** argv) {
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<double> dist(0.0, 1.0);
	for( int N=10; N<=10e6; N*=10) {
		double count = 0.0;
		for( int i=0; i<N; i++) {
			double x = dist(mt);
			double y = dist(mt);
			if( x*x + y*y < 1 ) {
				count += 1.0;
			}
		}
		double pi = 4*count/N;
		std::cout << "N=" << N << "\tpi=" << pi << std::endl;
	}
	return 0;
}