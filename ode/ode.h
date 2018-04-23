//
// Created by yj374 on 4/22/18.
//

#ifndef ECE4960_SP18_ODE_H
#define ECE4960_SP18_ODE_H

#include <ode/functor.h>

typedef std::vector<double> vec;

enum ode_methods {
	frd_euler,
	rk34,
	rk34_adap
};

struct ode_options{
	int rank;
	double dt;
	double t_end;
	std::vector<double> x_init;
	std::vector<double> params;
};

// snapshot of every step
struct ode_step {
	int iter;
	vec x;
};

class ode {
	ode_methods method;
	ode_options opt;
	std::vector<ode_step> steps;
	functor* f;
	int iter;
private:
	/// increment function of forward euler method
	/// \return increment vector
	vec incr_frd_euler();
	
	/// increment function of rk34
	/// \param is_adap true to enable adaptive method, false to disable it
	/// \return increment vector
	vec incr_rk34(bool is_adap);
	
public:
	
	ode(ode_methods method_,
	    ode_options opt_,
	    functor* f);
	bool step();
	vec incr();
	ode_step step_at_idx(int idx);
};



#endif //ECE4960_SP18_ODE_H
