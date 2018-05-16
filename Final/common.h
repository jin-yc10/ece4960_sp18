//
// Created by yj374 on 5/14/18.
//

#ifndef ECE4960_SP18_COMMON_H
#define ECE4960_SP18_COMMON_H

#include <fstream>
#include <cmath>
#include <ode/ode.h>
#include <ode/functor.h>

// shared functions goes here

/// output v_in val according to t
/// \param t time in us
/// \return voltage in V
double change_t = 1.0; // us
double v_in(double t) {
	if( t<change_t ) {
		return 0.0;
	} else {
		return 5.0;
	}
}

const double Is = 5e-3; // mA
double K = 0.7;
double Vth = 1.0;       // V
double VT = 26e-3;      // V
double VDD = 5.0;       // V
double RG = 10, RL = 10;// kOhm
double C1 = 1, C2 = 1;  // pF

// function id_ekv
double id_ekv(double v1, double v2) {
	double l1 = std::log(1+std::exp(K*(v1 - Vth)/(2.0*VT)));
	double l2 = std::log(1+std::exp((K*(v1 - Vth)-v2)/(2.0*VT)));
	return Is * l1 * l1 - Is * l2 * l2;
}

class kcl : public ode_functor {
	double Rfb = 1e3; // kOhm
public:
	kcl() {}
	
	void set_params(std::vector<double> params_) {
		Rfb = params_[0];
	}
	
	void operator()(std::vector<double> in, std::vector<double>& out) {
		double v1 = in[0], v2 = in[1], t = in[2];
		out.resize(2);
		double v_in_t = v_in(t);
		out[0] = (1/-C1)*(v1/RG - v_in_t/RG + (v1-v2)/Rfb);
		double val = id_ekv(v1, v2);
		out[1] = (1/-C2)*(v2/RL + val - VDD/RL + (v2-v1)/Rfb);
	}
};

void save_step(const ode_step& s, std::ofstream& out) {
	out << s.iter << " " << s.run_time*1e6 << " ";
	for(auto ix: s.x ) {
		out << ix << " ";
	}
	out << std::endl;
}

#endif //ECE4960_SP18_COMMON_H
