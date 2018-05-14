#include <general/functor.h>
#include <lsq/lsq.h>
#include <dlib/optimization.h>
#include <cmath>
#include <random>

using namespace dlib;

typedef matrix<double,2,1> input_vector;
typedef matrix<double,3,1> parameter_vector;

double model (
    const input_vector& input,
    const parameter_vector& params
) {
    const double p0 = params(0);
    const double p1 = params(1);
    const double p2 = params(2);

    const double i0 = input(0);
    const double i1 = input(1);

    const double temp = 
		p0 * i0 * i0 + 
		p1 * std::exp(i1) + 
		p2;

    return temp*temp;
}

double residual (
    const std::pair<input_vector, double>& data,
    const parameter_vector& params
) {
    return model(data.first, params) - data.second;
}

// ===============================

class residual2 : public general::functor {
public:
	residual2(int n_in) :general::functor(n_in) {}
	void set_params(std::vector<double> params_) {
		this->params = params_;
	}
	void operator()(std::vector<double> in, double& out) {
		double x0 = params[0];
		double x1 = params[1];
		double y = params[1];
		double a0 = in[0];
		double a1 = in[1];
		double a2 = in[2];
		double r_ = a0 * x0 * x0 + a1 * std::exp(x1) + a2 - y;
		out = r_ * r_;
	}
};

class model2 : public general::functor {
public:
	model2(int n_in):general::functor(n_in) {}
	
	void set_params(std::vector<double> params_) {
		this->params = params_;
	}

	void operator()(std::vector<double> in, double& out) {
		out = params[0]*in[0]*in[0]+params[1]*std::exp(in[1])+params[2];
	}

	void dv_manual(std::vector<double> in, cv::Mat& J_out) {
		J_out = cv::Mat(n_in, 1, CV_64F);
		J_out.at<double>(0, 0) = 2.0 * params[0] * in[0];
		J_out.at<double>(1, 0) = params[1] * std::exp(in[1]);
	}
};

class V : public general::functor {
	int n_samples;
	std::vector<residual2*> residuals;
public:
	V(int n_in) :general::functor(n_in) {}
	~V() {}

	void set_params(std::vector<double> params_) {
		this->params = params_;
		n_samples = (int)params[0]; // potential numerical problem?
		int idx = 1;
		for (int i = 0; i<n_samples; i++) {
			residual2* res = new residual2(3);
			std::vector<double> p_;
			p_.push_back(params_[idx]);
			p_.push_back(params_[idx + 1]);
			p_.push_back(params_[idx + 2]);
			idx += 3;
			res->set_params(p_);
			this->residuals.push_back(res);
		}
	}

	void operator()(std::vector<double> in, double& out) {
		double c0 = in[0];
		double m = in[1];
		double v_sum = 0.0;
		for (int i = 0; i<n_samples; i++) {
			double out_res = 0.0;
			(*(this->residuals[i]))(in, out_res);
			v_sum += out_res;
		}
		out = v_sum;
	}
};

int main() {
  const parameter_vector params = 10*randm(3,1);
  std::cout << "params: " << trans(params) << std::endl;
  std::vector<std::pair<input_vector, double> > data_samples;
  input_vector input;
  int n_sample = 10;
  std::vector<double> params2(1 + 3 * n_sample);
  params2[0] = n_sample;
  for (int i = 0; i < n_sample; ++i) {
      input = 10 * randm(2,1);
      const double output = model(input, params);
	  params2[i * 3 + 1] = input(0);
	  params2[i * 3 + 2] = input(1);
	  params2[i * 3 + 3] = output;
      // save the pair
      data_samples.push_back(std::make_pair(input, output));
  }

  parameter_vector x;
  x = 1;
  std::cout << "initial parameters: " << x << std::endl;
  solve_least_squares(objective_delta_stop_strategy(1e-7).be_verbose(), 
                        residual,
                        derivative(residual),
                        data_samples,
                        x);

  // Now x contains the solution.  If everything worked it will be equal to params.
  std::cout << "inferred parameters: "<< x << std::endl;
  std::cout << "solution error:      "<< x - params << std::endl;
  std::cout << std::endl;

  V* v_ = new V(3);
  v_->set_params(params2);

  lsq::lsq_config cfg;
  cfg.a = { 1, 1, 1 };
  cfg.tol = 1e-7;
  cfg.max_iter = (int)1e5;
  cfg.n_var = 3;
  cfg.V = v_;
  lsq::solver solver(cfg);
  solver.solve();
  
  //model2* l = new model2(2);
  //l->set_params(params2);
  //std::vector<double> in = { 3.0, 5.0 };
  //double out;
  //(*l)(in, out);
  //cv::Mat J;
  //(*l).dv(in, J);
  //cv::Mat J2;
  //(*l).dv_manual(in, J2);
  //std::cout << out << std::endl;
  //std::cout << J << std::endl;
  //std::cout << J2 << std::endl;
  return 0;
}