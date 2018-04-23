//
// Created by yj374 on 4/22/18.
//

#include <ode/functor.h>
#include <ode/ode.h>

#include <cmath>
#include <iostream>
#include <fstream>

std::ofstream output_file;

// function F
class test_function : public functor {
public:
	test_function() {}
	void set_params(std::vector<double> params_) {}
	///
	/// \param in in[0] = x, in[1] = t
	/// \param out out[0] = f(x, t)
	void operator()(std::vector<double> in, std::vector<double>& out) {
		double x = in[0], t = in[1];
		out.resize(1);
		out[0] = 4.0*std::exp(0.8*t) - 0.5*x;
	}
};

// function Ground Truth
class gt_function : public functor {
public:
	gt_function() {}
	void set_params(std::vector<double> params_) {}
	
	///
	/// \param in in[0] = x, in[1] = t
	/// \param out out[0] = f(x, t)
	void operator()(std::vector<double> in, std::vector<double>& out) {
		double x = in[0], t = in[1];
		out.resize(1);
		out[0] = (4.0/1.3)*(std::exp(0.8*t)-std::exp(-0.5*t)) + 2.0*std::exp(-0.5*t);
	}
};

/// calculate the relative error in percent
/// \param v
/// \param g
/// \return
double error(double v, double g) {
	return 100.0*std::abs(v-g)/g;
}

void test(ode_methods method) {
	std::cout << std::endl;
	ode_options opt;
	opt.x_init = { 2.0, 0.0 };
	opt.t_end = 10.0;
	opt.dt = 0.5;
	opt.rank = 1;
	if( method == rk34_adap ) {
		opt.params = {1e-2, 1e-6}; // TOL1, TOL2
	}
	test_function* f = new test_function();
	gt_function g;
	ode _ode(method, opt, f);
	do {
		auto s = _ode.step_at_idx(-1);
		vec g_out;
		g(s.x, g_out);
		output_file << s.iter << " " << s.x[1] << " " << s.x[0] << " "
		            << g_out[0] << std::endl;
		std::cout << error(s.x[0], g_out[0]) << std::endl;
	} while( _ode.step() );
	delete f;
}

int main(int argc, char** argv) {
	output_file.open("03.txt", std::ofstream::out);
	output_file << "frd_euler" << std::endl;
	test(frd_euler);
	output_file << "rk34" << std::endl;
	test(rk34);
	output_file << "rk34_adap" << std::endl;
	test(rk34_adap);
	output_file.close();
	return 0;
}
