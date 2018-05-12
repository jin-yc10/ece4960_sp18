//
// Created by yj374 on 5/11/18.
//

#ifndef ECE4960_SP18_FUNCTOR_H
#define ECE4960_SP18_FUNCTOR_H

#include <vector>
#include <opencv2/opencv.hpp>

namespace lsq {
	// preturb factor
	const double P = 1e-4;
	
	class functor {
	protected:
		std::vector<double> params;
		int n_in;
	public:
		functor(int n_in): n_in(n_in) {};
		virtual void set_params(std::vector<double> params_) {};
		
		virtual void operator()(std::vector<double> in, double& out) {};
		
		virtual void dv(std::vector<double> in, cv::Mat& J_out) {
			J_out = cv::Mat(n_in, 1, CV_64F);
			double out_x0;
			(*this)(in, out_x0);
			for(int i=0; i<n_in; i++) {
				std::vector<double> in_(in);
				in_[i] += in[i]*P;
				double out_;
				(*this)(in_, out_);
				J_out.at<double>(i, 0) = (out_ - out_x0) / (in[i]*P);
			}
		};
		
		virtual void dv2(std::vector<double> in, cv::Mat& H_out) {
			H_out = cv::Mat(n_in, n_in, CV_64F);
			double x0;
			(*this)(in, x0);
			for(int i=0; i<n_in; i++) {
				for(int j=0; j<n_in; j++) {
					double x1, x2;
					std::vector<double> in1, in2;
					in1 = in; in2 = in;
					double dx1, dx2;
					dx1 = in[i]*P; dx2 = in[j]*P;
					in1[i] += dx1;
					in2[j] += dx2;
					(*this)(in1, x1); (*this)(in2, x2);
					H_out.at<double>(i, j) = 6*(x2-2*x1+x0)/(dx1 * dx2);
	      }
			}
		};
	}; // class functor

} // namespace lsq

#endif //ECE4960_SP18_FUNCTOR_H
