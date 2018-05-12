//
// Created by yj374 on 5/11/18.
//

#include <cmath>
#include <lsq/functor.h>
#include <lsq/lsq.h>
#include <fstream>

const double Vsb = 0.0;       //S is shorted to B
const double VT = 26E-3;      //VT in volts

double id_ekv(double Vgb, double Vdb, double Is, double K, double Vth) {
	double l1 = std::log(1+std::exp((K*(Vgb - Vth)-Vsb)/(2.0*VT))); // If
	double l2 = std::log(1+std::exp((K*(Vgb - Vth)-Vdb)/(2.0*VT))); // Ir
	return Is * (l1 * l1 - l2 * l2);
} // Id

class residual: public lsq::functor {
public:
	residual(int n_in):lsq::functor(n_in) {}
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

class residual_normalized: public lsq::functor {
public:
	residual_normalized(int n_in):lsq::functor(n_in) {}
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


class V : public lsq::functor {
	int n_samples;
	std::vector<residual*> residuals;
public:
	V(int n_in):lsq::functor(n_in) {}
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
//			std::cout << p_[0] << " " << p_[1] << " " << p_[2] << std::endl;
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

int main() {
	std::vector<double> V_params;
	int n_samples = 1010;
	V_params.push_back((double)n_samples);
	std::ifstream nmos("data/outputNMOS.txt");
	std::string nothing;
//	nmos >> nothing >> nothing >> nothing; // skip first line
	for(int i=0; i<n_samples; i++) {
		double v1, v2, v3;
		nmos >> v1 >> v2 >> v3;
		V_params.push_back(v1);V_params.push_back(v2);V_params.push_back(v3);
	}
	nmos.close();
	
	V* v_ = new V(3);
	v_->set_params(V_params);
	
	lsq::lsq_config cfg;
	cfg.a = { 1e-7, 1, 1 }; // Is, K, Vth
	cfg.a_pre = { 0.0, 0.0, 0.0 };
	cfg.m = lsq::newton;
	cfg.tol = 1e-7;
	cfg.max_iter = (int)1e5;
	cfg.n_var = 3;
	cfg.V = v_;
	lsq::solver solver(cfg);
	solver.solve();
	
	delete v_;
	return 0;
}