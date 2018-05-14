//
// Created by yj374 on 3/30/18.
//

#include <general/functor.h>
#include <lsq/lsq.h>
#include <cmath>
#include <random>

std::mt19937 rng;

class power_law : public general::functor {
public:
	power_law(int n_in):general::functor(n_in) {}
	
	void set_params(std::vector<double> params_) {
		this->params = params_;
	}

	void operator()(std::vector<double> in, double& out) {
		double x = in[0];
		double c0 = params[0];
		double m = params[1];
		out = c0*std::pow(x, m);
	}
};

class residual: public general::functor {
public:
	residual(int n_in):general::functor(n_in) {}
	void set_params(std::vector<double> params_) {
		this->params = params_;
	}
	void operator()(std::vector<double> in, double& out) {
		double x = params[0];
		double y = params[1];
		double c0 = in[0];
		double m = in[1];
		double r_ = c0*std::pow(x, m) - y;
		out = r_*r_;
	}
};

class V : public general::functor {
	int n_samples;
	std::vector<residual*> residuals;
public:
	V(int n_in):general::functor(n_in) {}
	~V() {
	
	}
	
	void set_params(std::vector<double> params_) {
		this->params = params_;
		n_samples = (int)params[0]; // potential numerical problem?
		int idx = 1;
		for( int i=0; i<n_samples; i++) {
			residual* res = new residual(1);
			std::vector<double> p_;
			p_.push_back(params_[idx]);
			p_.push_back(params_[idx+1]);
			idx += 2; // x0, y0, x1, y1, ...
			res->set_params(p_);
			this->residuals.push_back(res);
		}
	}
	
	void operator()(std::vector<double> in, double& out) {
		double c0 = in[0];
		double m = in[1];
		double v_sum = 0.0;
		for(int i=0; i<n_samples; i++) {
			double out_res = 0.0;
			(*(this->residuals[i]))(in, out_res);
			v_sum += out_res;
		}
		out = v_sum;
	}
};

int main(int argc, char** argv) {
	// std::random_device()()
	rng.seed(0);
	std::vector<double> S_measureds;
	std::vector<double> Xs;
	
	// prepare the measured data
	std::vector<double> pl_params = {15, -0.6};
	std::vector<double> V_params;
	power_law pl(1); pl.set_params(pl_params);
	int n_samples = 10;
	V_params.push_back((double)n_samples);
	double noise = 5; // percent
	std::uniform_real_distribution<double> dis(-noise, noise);
	for(int i=0; i<n_samples; i++) {
		std::vector<double> x;
		x.push_back((double)(i+1));
		double v;
		pl(x, v);
		Xs.push_back(x[0]);
		double val = v * (1.0 + dis(rng) / 100.0);
		S_measureds.push_back(val);
		V_params.push_back((double)(i+1));
		V_params.push_back(val);
	}
	V* v_ = new V(2);
	v_->set_params(V_params);
	
	lsq::lsq_config cfg;
	cfg.a = { 10, -0.7 };
	cfg.tol = 1e-6;
	cfg.max_iter = (int)1e5;
	cfg.n_var = 2;
	cfg.V = v_;
	lsq::solver solver(cfg);
	solver.solve();
	
	delete v_;
	return 0;
}