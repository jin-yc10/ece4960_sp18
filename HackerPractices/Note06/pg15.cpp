//
// Created by yj374 on 4/10/18.
//

#include <cmath>
#include <iostream>
#include <vector>

/* OUTPUT
===============
dt = 0.5
1	1	1
0.606531	0.666667	0.6
0.367879	0.444444	0.36
0.22313	0.296296	0.216
0.135335	0.197531	0.1296
0.082085	0.131687	0.07776
0.0497871	0.0877915	0.046656
0.0301974	0.0585277	0.0279936
0.0183156	0.0390184	0.0167962
0.011109	0.0260123	0.0100777
0.00673795	0.0173415	0.00604662
0.00408677	0.011561	0.00362797
0.00247875	0.00770735	0.00217678
0.00150344	0.00513823	0.00130607
0.000911882	0.00342549	0.000783642
0.000553084	0.00228366	0.000470185
0.000335463	0.00152244	0.000282111
0.000203468	0.00101496	0.000169267
0.00012341	0.000676639	0.00010156
7.48518e-05	0.000451093	6.0936e-05
4.53999e-05	0.000300729	3.65616e-05
2.75364e-05	0.000200486	2.1937e-05
1.67017e-05	0.000133657	1.31622e-05
1.01301e-05	8.91048e-05	7.8973e-06
6.14421e-06	5.94032e-05	4.73838e-06
3.72665e-06	3.96021e-05	2.84303e-06
2.26033e-06	2.64014e-05	1.70582e-06
1.37096e-06	1.76009e-05	1.02349e-06
8.31529e-07	1.1734e-05	6.14094e-07
5.04348e-07	7.82264e-06	3.68457e-07
3.05902e-07	5.2151e-06	2.21074e-07
1.85539e-07	3.47673e-06	1.32644e-07
1.12535e-07	2.31782e-06	7.95866e-08
6.8256e-08	1.54521e-06	4.7752e-08
4.13994e-08	1.03014e-06	2.86512e-08
2.511e-08	6.86761e-07	1.71907e-08
1.523e-08	4.57841e-07	1.03144e-08
9.23745e-09	3.05227e-07	6.18865e-09
5.6028e-09	2.03485e-07	3.71319e-09
3.39827e-09	1.35657e-07	2.22792e-09
2.06115e-09	9.04377e-08	1.33675e-09
 ===============
dt = 1
1	1	1
0.367879	0.5	0.333333
0.135335	0.25	0.111111
0.0497871	0.125	0.037037
0.0183156	0.0625	0.0123457
0.00673795	0.03125	0.00411523
0.00247875	0.015625	0.00137174
0.000911882	0.0078125	0.000457247
0.000335463	0.00390625	0.000152416
0.00012341	0.00195312	5.08053e-05
4.53999e-05	0.000976562	1.69351e-05
1.67017e-05	0.000488281	5.64503e-06
6.14421e-06	0.000244141	1.88168e-06
2.26033e-06	0.00012207	6.27225e-07
8.31529e-07	6.10352e-05	2.09075e-07
3.05902e-07	3.05176e-05	6.96917e-08
1.12535e-07	1.52588e-05	2.32306e-08
4.13994e-08	7.62939e-06	7.74352e-09
1.523e-08	3.8147e-06	2.58117e-09
5.6028e-09	1.90735e-06	8.60392e-10
2.06115e-09	9.53674e-07	2.86797e-10
*/
std::vector<double> gt_f(double f0, double dt, double t_start, double t_end) {
	std::vector<double> fs;
	double f = f0;
	double t = t_start;
	while(t <= t_end) {
		fs.push_back(std::exp(-t));
		t += dt;
	}
	return fs;
}

std::vector<double> backward_f(double f0, double dt, double t_start, double t_end) {
	std::vector<double> fs;
	double f = f0;
	double t = t_start;
	while(t <= t_end) {
		fs.push_back(f);
		f = f/(1+dt);
		t += dt;
	}
	return fs;
}

std::vector<double> trapezoidal_f(double f0, double dt, double t_start, double t_end) {
	std::vector<double> fs;
	double f = f0;
	double t = t_start;
	while(t <= t_end) {
		fs.push_back(f);
		f = f*(2-dt)/(2+dt);
		t += dt;
	}
	return fs;
}

void test_case(double dt) {
	double f0 = 1.0;
	auto gt = gt_f(f0, dt, 0.0, 20.0);
	auto bf = backward_f(f0, dt, 0.0, 20.0);
	auto tf = trapezoidal_f(f0, dt, 0.0, 20.0);
	std::cout << " =============== " << std::endl;
	std::cout << "dt = " << dt << std::endl;
	for( int i=0; i<gt.size(); i++) {
		std::cout << gt[i] << "\t" << bf[i] << "\t" << tf[i] << std::endl;
	}
}

int main(int argc, char** argv) {
	test_case(0.5);
	test_case(1.0);
	return 0;
}