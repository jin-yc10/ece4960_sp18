#include <lsq/functor.h>
#include <lsq/lsq.h>
#include <cmath>
#include <random>

class linear : public lsq::functor {
public:
	linear(int n_in):lsq::functor(n_in) {}
	
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

int main() {
  linear* l = new linear(2);
  std::vector<double> params = { 1.0, 2.0, 3.0 };
  l->set_params(params);
  std::vector<double> in = { 3.0, 5.0 };
  double out;
  (*l)(in, out);
  cv::Mat J;
  (*l).dv(in, J);
  cv::Mat J2;
  (*l).dv_manual(in, J2);
  std::cout << out << std::endl;
  std::cout << J << std::endl;
  std::cout << J2 << std::endl;
  return 0;
}