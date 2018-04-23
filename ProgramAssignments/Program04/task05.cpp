//
// Created by yj374 on 4/22/18.
//

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
double Is = 5e-3; // mA
double K = 0.7;
double Vth = 1.0; // V
double VT = 26e-3; // V
double VDD = 5.0;
double RG = 10, RL = 10; // kOhm
double C1 = 1, C2 = 1; // pF

// function id_ekv
double id_ekv(double v1, double v2) {
	double l1 = std::log(1+std::exp(K*(v1 - Vth)/(2.0*VT)));
	double l2 = std::log(1+std::exp((K*(v1 - Vth)-v2)/(2.0*VT)));
	return Is * l1 * l1 - Is * l2 * l2;
}

// function for figure 5(a)
class fig5a : public functor {
public:
	fig5a() {}
	void set_params(std::vector<double> params_) {}
	void operator()(std::vector<double> in, std::vector<double>& out) {
		double v1 = in[0], v2 = in[1], t = in[2];
		out.resize(2);
		out[0] = -v1/(RG*C1)+i(t)/(C1);
		out[1] = -id_ekv(v1, v2)/C2 - v2/(RL*C2) + VDD / (RL*C2);
	}
};

void test(ode_methods method) {
	ode_options opt;
	opt.x_init = { 2.5, 2.5, 0.0 };
	opt.t_end = 100.0;
	opt.dt = 0.2;
	opt.rank = 2;
	if( method == rk34_adap ) {
		opt.params = {1e-2, 1e-6}; // TOL1, TOL2
	}
	fig5a* f = new fig5a();
	ode _ode(method, opt, f);
	do {
		auto s = _ode.step_at_idx(-1);
		output_file << s.iter << " " << s.x[2] << " " << s.x[0] << " " << s.x[1] << std::endl;
	} while( _ode.step() );
	delete f;
	
}

int main(int argc, char** argv) {
	output_file.open("05.txt", std::ofstream::out);
	output_file << "frd_euler" << std::endl;
	test(frd_euler);
	output_file << "rk34" << std::endl;
	test(rk34);
	output_file << "rk34_adap" << std::endl;
	test(rk34_adap);
	output_file.close();
	return 0;
}
