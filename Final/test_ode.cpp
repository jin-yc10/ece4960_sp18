//
// Created by yj374 on 5/14/18.
//

#include <ode/ode.h>
#include <ode/functor.h>
#include "common.h"

#include <iostream>

int main() {
	
	ode_options opt;
	opt.x_init = { 0.0, 5.0, 0.0 };
	opt.t_end = 100.0e3; // ns?
	opt.dt = 0.01;
	opt.rank = 2;
	kcl* f = new kcl();
	std::vector<double> params = { 1e3 }; // set Rfb = 1e3 kOhm
	f->set_params(params);
	
	ode _ode(frd_euler, opt, f);
	do {
		auto s = _ode.step_at_idx(-1);
		std::cout << s.iter << " " << s.run_time*1e6 << " ";
		for(auto ix: s.x ) {
			std::cout << ix << " ";
		}
		std::cout << std::endl;
		if( s.x[1] < 1.0 ) {
			auto s_pre = _ode.step_at_idx(-2);
			double t_pre = s_pre.x[2];
			double v_pre = s_pre.x[1];
			double t_now = s.x[2];
			double v_now = s.x[1];
			double tx = (t_now-(t_now-t_pre)*(1.0-v_now)/(v_pre-v_now));
			std::cout << tx << std::endl;
			break;
		}
	} while( _ode.step() );
	delete f;
	
	return 0;
}
