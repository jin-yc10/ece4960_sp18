//
// Created by yj374 on 5/11/18.
//

#include <cmath>
#include <general/functor.h>
#include <lsq/lsq.h>
#include <fstream>

#include <dlib/optimization.h>

const double Vsb = 0.0;       //S is shorted to B
const double VT = 26E-3;      //VT in volts

double id_ekv(double Vgb, double Vdb, double Is, double K, double Vth) {
	double l1 = std::log(1+std::exp((K*(Vgb - Vth)-Vsb)/(2.0*VT))); // If
	double l2 = std::log(1+std::exp((K*(Vgb - Vth)-Vdb)/(2.0*VT))); // Ir
	return Is * (l1 * l1 - l2 * l2);
} // Id

class residual: public general::functor {
public:
	residual(int n_in):general::functor(n_in) {}
	void set_params(std::vector<double> params_) {
		this->params = params_;
	}
	void operator()(std::vector<double> in, double& out) {
		double Vgb = params[0];
		double Vdb = params[1];
		double Id = params[2];
		double Is = in[0];
		double K = in[1];
		double Vth = in[2];
		double r_ = id_ekv(Vgb, Vdb, Is, K, Vth) - Id;
		out = r_*r_;
	}
};

class residual_normalized: public general::functor {
public:
	residual_normalized(int n_in):general::functor(n_in) {}
	void set_params(std::vector<double> params_) {
		this->params = params_;
	}
	void operator()(std::vector<double> in, double& out) {
		double Vgb = params[0];
		double Vdb = params[1];
		double Id = params[2];
		double Is = in[0];
		double K = in[1];
		double Vth = in[2];
		double r_ = id_ekv(Vgb, Vdb, Is, K, Vth)/Id - 1;
		out = r_*r_;
	}
};


class V : public general::functor {
	int n_samples;
	std::vector<residual*> residuals;
public:
	V(int n_in):general::functor(n_in) {}
	~V() {
		for( int i=0; i<n_samples; i++) {
			delete this->residuals[i];
		}
	}
	// Vgs     Vds     Ids
	void set_params(std::vector<double> params_) {
		this->params = params_;
		n_samples = (int)params[0]; // potential numerical problem?
		int idx = 1;
		for( int i=0; i<n_samples; i++) {
			residual* res = new residual(3);
			std::vector<double> p_;
			p_.push_back(params_[idx]);
			p_.push_back(params_[idx+1]);
			p_.push_back(params_[idx+2]);
			idx += 3; // Vgb, Vdb, Id, ...
			res->set_params(p_);
			this->residuals.push_back(res);
		}
	}
	
	void operator()(std::vector<double> in, double& out) {
		double v_sum = 0.0;
		for(int i=0; i<n_samples; i++) {
			double out_res = 0.0;
			(*(this->residuals[i]))(in, out_res);
			v_sum += out_res;
		}
		out = v_sum;
	}
};

using namespace dlib;

typedef matrix<double, 2, 1> input_vector;
typedef matrix<double, 3, 1> parameter_vector;

double model(
	const input_vector& input,
	const parameter_vector& params
) {
	double Vgb = input(0);
	double Vdb = input(1);
	//double Id = input(2);
	double Is = params(0);
	double K = params(1);
	double Vth = params(2);
	return id_ekv(Vgb, Vdb, Is, K, Vth);
}

double residual_(
	const std::pair<input_vector, double>& data,
	const parameter_vector& params
) {
	return model(data.first, params) - data.second;
}

int main(int argc, char** argv) {

	std::vector<std::pair<input_vector, double> > data_samples;
	input_vector input;

	std::vector<double> V_params;
	int n_samples = 1010;
	V_params.push_back((double)n_samples);
	std::ifstream nmos("C:/Dev/ece4960_sp18/data/outputNMOS.txt");
	std::string nothing;
//	nmos >> nothing >> nothing >> nothing; // skip first line
	for(int i=0; i<n_samples; i++) {
		double v1, v2, v3;
		nmos >> v1 >> v2 >> v3;
		V_params.push_back(v1);V_params.push_back(v2);V_params.push_back(v3);
		input = randm(2, 1);
		input(0) = v1; input(1) = v2;
		const double output = v3;
		data_samples.push_back(std::make_pair(input, output));
	}
	nmos.close();
	
	V* v_ = new V(3);
	v_->set_params(V_params);
	
	lsq::lsq_config cfg;
	cfg.a = { 1e-6, 1, 1 }; // Is, K, Vth
	cfg.tol = 1e-7;
	cfg.max_iter = (int)1e5;
	cfg.n_var = 3;
	cfg.V = v_;
	lsq::solver solver(cfg);
	solver.solve();
	
	parameter_vector x;
	x = { 1e-7, 1, 1 };
	solve_least_squares(objective_delta_stop_strategy(1e-7).be_verbose(),
		residual_,
		derivative(residual_),
		data_samples,
		x);
	std::cout << "inferred parameters: " << x << std::endl;

	delete v_;
	return 0;
}