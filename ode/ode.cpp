//
// Created by yj374 on 4/22/18.
//

#include <iostream>
#include "ode.h"

ode::ode(ode_methods method_, ode_options opt_, functor* f_)
		: method(method_), opt(opt_), f(f_), iter(0){
	ode_step init_s;
	init_s.iter = iter;
	init_s.x = opt_.x_init;
	this->steps.push_back(init_s);
}

bool ode::step() {
	auto last = this->step_at_idx(-1);
	ode_step s;
	s.iter = ++iter;
	s.x.resize(opt.rank+1);
	s.x[opt.rank] = last.x[opt.rank]+opt.dt;
	vec incr_ = incr();
	if( s.x[opt.rank] > opt.t_end ) {
		return false;
	}
	for( int i=0; i<opt.rank; i++) {
		s.x[i] = last.x[i] + incr_[i];//*opt.dt;
	}
	this->steps.push_back(s);
	return true;
}

vec ode::incr_frd_euler() {
	std::vector<double> out(opt.rank);
	auto x = this->step_at_idx(-1).x;
	(*f)(x, out);
	for(int i=0; i<opt.rank; i++)
		out[i] *= opt.dt;
	return out;
}

vec ode::incr_rk34(bool is_adap) {
	std::vector<double> out(opt.rank);
	auto x = this->step_at_idx(-1).x;
	vec x_(opt.rank+1);
	
	for(int i=0; i<opt.rank; i++)
		x_[i] = x[i];
	x_[opt.rank] = x[opt.rank];
	vec k1(opt.rank+1), k2(opt.rank+1), k3(opt.rank+1), k4(opt.rank+1);
	(*f)(x_, k1); // k1 = f(ti, xi)
	
	for(int i=0; i<opt.rank; i++)
		x_[i] = x[i] + k1[i]*opt.dt/2.0;
	x_[opt.rank] = x[opt.rank]+opt.dt/2.0;
	(*f)(x_, k2); // k2 = f(ti+h/2, xi+k1*h/2)
	
	for(int i=0; i<opt.rank; i++)
		x_[i] = x[i] + k2[i]*opt.dt*3.0/4.0;
	x_[opt.rank] = x[opt.rank]+opt.dt*3.0/4.0;
	(*f)(x_, k3); // k3 = f(ti+0.75h, xi+k2*h*0.75)
	
	for(int i=0; i<opt.rank; i++)
		x_[i] = x[i] + k3[i]*opt.dt;
	x_[opt.rank] = x[opt.rank]+opt.dt;
	(*f)(x_, k4); // k4 = f(ti+h, xi+k3*h)
	
	for(int i=0; i<opt.rank; i++) {
		out[i] = (7.0*k1[i]+6.0*k2[i]+8.0*k3[i]+3.0*k4[i])*opt.dt/24.0;
	}
	
	if( !is_adap ) {
	  // pass
	} else {
		vec ei(opt.rank+1);
		for(int i=0; i<opt.rank; i++) {
			ei[i] = (-5.0*k1[i]+6.0*k2[i]+8.0*k3[i]-9.0*k4[i])*opt.dt/72.0;
		}
		double nx = 0.0, err = 0.0;
		for(int i=0; i<opt.rank; i++) {
			double x_i1 = x[i]+out[i]*opt.dt;
			nx += x_i1*x_i1;
			err += ei[i]*ei[i];
		}
		if( err > opt.params[0] ) opt.dt /= 2.0;
		if( err < opt.params[1] ) opt.dt *= 2.0;
	}
	return out;
}

vec ode::incr() {
	switch ( this->method ) {
		case frd_euler:
			return this->incr_frd_euler();
		case rk34:
			return this->incr_rk34(false);
		case rk34_adap:
			return this->incr_rk34(true);
		default:
			return vec();
	}
}

ode_step ode::step_at_idx(int idx) {
	if( idx < 0 ) {
		idx += this->steps.size();
		if ( idx < 0 ) {
			// if still below zero
			return ode_step();
		}
	}
	return this->steps[idx];
}