//
// Created by yj374 on 4/22/18.
//

#include <ode/functor.h>
#include <ode/ode.h>
#include "utils.h"

#include <cmath>
#include <iostream>
#include <fstream>

std::ofstream output_file;
//#define output_file std::cout
double R1 = 10, R2 = 10, R3 = 10; // kOhm
double C1 = 1, C2 = 1; // pF

// function for figure 3
class fig3 : public functor {
public:
	fig3() {}
	void set_params(std::vector<double> params_) {}
	void operator()(std::vector<double> in, std::vector<double>& out) {
		double v1 = in[0], v2 = in[1], t = in[2];
		out.resize(2);
		out[0] = -v1*(1.0/(C1*R1)+1.0/(C1*R2))+v2/(C1*R2)+i(t)/C1;
		out[1] = v1/(C2*R2)-v2*(1.0/(C2*R2)+1.0/(C2*R3));
	}
};

void test(ode_methods method) {
	ode_options opt;
	opt.x_init = { 0.0, 0.0, 0.0 };
	opt.t_end = 100.0;
	opt.dt = 0.2;
	opt.rank = 2;
	if( method == rk34_adap ) {
		opt.params = {1e-2, 1e-14}; // TOL1, TOL2
	}
	fig3* f = new fig3();
	ode _ode(method, opt, f);
	do {
		auto s = _ode.step_at_idx(-1);
		output_file << s.iter << " " << s.x[2] << " " << s.x[0] << " " << s.x[1] << std::endl;
	} while( _ode.step() );
	delete f;
}

int main(int argc, char** argv) {
	output_file.open("04.txt", std::ofstream::out);
	output_file << "frd_euler" << std::endl;
	test(frd_euler);
	output_file << "rk34" << std::endl;
	test(rk34);
	output_file << "rk34_adap" << std::endl;
	test(rk34_adap);
	output_file.close();
	return 0;
}
