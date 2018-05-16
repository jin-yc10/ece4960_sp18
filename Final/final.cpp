//
// Created by yj374 on 5/14/18.
//

#include <general/functor.h>
#include <nonlinear/find_min.h>
#include <ode/ode.h>
#include <ode/functor.h>

#include "common.h"

// general function of Rbf vs Tr
class rfb_tr: public general::functor {
public:
	rfb_tr(int n_in):general::functor(n_in) {}
	~rfb_tr() {}
	void set_params(std::vector<double> params_) {}
	void operator()(std::vector<double> in, double& out) {
		double rfb = in[0];
		ode_options opt;
		opt.x_init = { 0.0, 5.0, 0.0 }; // try to start from steady state
		opt.t_end = 100.0;
		opt.dt = 0.05;
		opt.rank = 2;
		kcl* f = new kcl();
		std::vector<double> params = { rfb };
		f->set_params(params);
		
		ode _ode(rk34, opt, f);
		double Tr = 1e20; // large enough
		do {
			auto s = _ode.step_at_idx(-1);
			if( s.x[1] < 1.0 ) {
				// linear interpolation to get "better" result
				auto s_pre = _ode.step_at_idx(-2);
				double t_pre = s_pre.x[2];
				double v_pre = s_pre.x[1];
				double t_now = s.x[2];
				double v_now = s.x[1];
				double tx = (t_now-(t_now-t_pre)*(1.0-v_now)/(v_pre-v_now));
				Tr = tx;
				break;
			}
		} while( _ode.step() );
		delete f;
		out = Tr;
	}
};

void search_newton() {
	std::cout << "----- search newton -----" << std::endl;
	// not working, since the dv function won't work due to the accuracy of ODE
	rfb_tr* func = new rfb_tr(1);
	
	nonlinear::find_min_config find_min_cfg;
	find_min_cfg.func = func;
	find_min_cfg.max_iter = 1e4;
	find_min_cfg.method = nonlinear::FIND_MIN_QUASI_NEWTON;
	find_min_cfg.n_var = 1;
	find_min_cfg.tol = 1e-5;
	find_min_cfg.x = { 1e3 }; // start from 1e3 kOhm
	find_min_cfg.verbose = 1;
	nonlinear::find_min* fm = new nonlinear::find_min(find_min_cfg);
	fm->solve();
}

void search_bisection() {
 // didn't implmented
}

void search_brute_force() {
	std::cout << "----- search brute force -----" << std::endl;
	rfb_tr* func = new rfb_tr(1);
	for(double rbf = 0.5e3; rbf <= 50e3; rbf += 10.0) {
		std::vector<double> in; in.push_back(rbf);
		double Tr;
		(*func)(in, Tr);
		std::cout << rbf << " " << Tr << std::endl;
	}
}

int main() {
	timer_util tm;
	tm.tick();
	search_newton();
	tm.tock("Newton Method");
	
	search_bisection();
	
	tm.tick();
	search_brute_force();
	tm.tock("Brute Force");
	return 0;
}