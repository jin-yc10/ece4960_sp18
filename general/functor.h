//
// Created by yj374 on 5/11/18.
//

#ifndef ECE4960_SP18_FUNCTOR_H
#define ECE4960_SP18_FUNCTOR_H

#include <vector>
#include <opencv2/opencv.hpp>

namespace general {
	// preturb factor
	const double P = 1e-7;
	
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
				double tmp = in[i];
				in_[i] = tmp + tmp * P;
				double delta_plus, delta_minus;
				(*this)(in_, delta_plus);
				in_[i] = tmp - tmp * P;
				(*this)(in_, delta_minus);
				J_out.at<double>(i, 0) = (delta_plus - delta_minus) / ((tmp + tmp * P) - (tmp - tmp * P));
			}
		};
		
		virtual void dv2(std::vector<double> in, cv::Mat& H_out) {
			H_out = cv::Mat(n_in, n_in, CV_64F);
			cv::Mat J;
			dv(in, J);
			/*
			\quad  ((f(x + d_j e_j + d_k e_k) - f(x + d_j e_j - d_k e_k)) -
			(f(x - d_j e_j + d_k e_k) - f(x - d_j e_j - d_k e_k)) /
			(4 d_j d_k)
			*/
			for (int j = 0; j<n_in; j++) {
				std::vector<double> in_(in);
				double dj = in[j] * P; double tj = in[j];
				double f_p2, f, f_n2;
				in_[j] = tj + 2 * dj; (*this)(in_, f_p2);
				in_[j] = tj;          (*this)(in_, f);
				in_[j] = tj - 2 * dj; (*this)(in_, f_n2);
				H_out.at<double>(j, j) = (f_p2 - 2 * f + f_n2) / (4 * dj * dj);
				for (int k = j+1; k<n_in; k++) {
					double dk = in[k] * P; double tk = in[k];
					double f_11, f_10, f_01, f_00;
					in_[j] = tj + dj; in_[k] = tk + dk; (*this)(in_, f_11);
					in_[j] = tj + dj; in_[k] = tk - dk; (*this)(in_, f_10);
					in_[j] = tj - dj; in_[k] = tk + dk; (*this)(in_, f_01);
					in_[j] = tj - dj; in_[k] = tk - dk; (*this)(in_, f_00);
					H_out.at<double>(j, k) = ((f_11 - f_10) - (f_01 - f_00)) / (4.0*dj*dk);
					H_out.at<double>(k, j) = H_out.at<double>(j, k);
				}
			}
		};
	}; // class functor
}; // namespace general

#endif //ECE4960_SP18_FUNCTOR_H
