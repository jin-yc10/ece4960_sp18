//
// Created by yj374 on 4/22/18.
//

#ifndef ECE4960_SP18_ODE_FUNCTOR_H
#define ECE4960_SP18_ODE_FUNCTOR_H

#include <vector>

class ode_functor {
protected:
	std::vector<double> params;
public:
	ode_functor() {};
	virtual void set_params(std::vector<double> params_) {};
	
	/// the base function, use polymorphism to define specific function
	/// \param in
	/// \param out
	virtual void operator()(std::vector<double> in, std::vector<double>& out) {};
};


#endif //ECE4960_SP18_ODE_FUNCTOR_H
