//
// Created by 金宇超 on 5/11/18.
//

#ifndef ECE4960_SP18_LSQ_H
#define ECE4960_SP18_LSQ_H

#include "functor.h"

// Least square solver
namespace lsq {
	
	enum method {
		newton,
		secant
	};
	
	struct lsq_config {
		functor* V;
		double tol;
		int max_iter;
		int n_measured;
		int n_var;
		std::vector<double> x;
		std::vector<double> a;
		std::vector<double> a_pre;
		method m;
	};
	
	struct lsq_report {
		double run_time;
		std::vector<double> opti_params;
	};
	
	class solver {
		lsq_config cfg;
		
		void solve_newton() {
			std::cout << "solve_newton" << std::endl;
			int iter_idx = 0;
			while( iter_idx < cfg.max_iter ) {
				
				// dx = -[H(x)]-1*dV(x)
				cv::Mat H, J;
				cfg.V->dv(cfg.a, J);
				cfg.V->dv2(cfg.a, H);
				cv::Mat H_inv = H.inv(cv::DECOMP_SVD);
				std::vector<double> d(cfg.n_var, 0.0);
				double d2 = 0.0;
				for(int i=0; i<cfg.n_var; i++) {
					for(int j=0; j<cfg.n_var; j++) {
						d[i] += -(H_inv.at<double>(j, i)*J.at<double>(j));
						d2 += d[i]*d[i];
					}
				}
				d2 = std::sqrt(d2);
				
				// check norm(x)
				if( d2 < cfg.tol ) {
					std::cout << "\nConverged" << std::endl;
					break;
				}
				for(int i=0; i<cfg.n_var; i++) {
					cfg.a[i] += d[i];
				}
				
				if( iter_idx % 100 == 0) {
					double val;
					(*cfg.V)(cfg.a, val);
					std::cout << "Iter " << iter_idx;
					std::cout << " | V(x) = " << val;
					std::cout << " ||d||2 = " << d2;
					std::cout << ", a = [";
					for(int i=0; i<cfg.n_var; i++) {
						std::cout << cfg.a[i] << " ";
					}
					std::cout << "]" << "\n";
				}
				iter_idx += 1;
			}
			
			if( iter_idx == cfg.max_iter ) {
				// exceed the maximum iteration
				std::cout << "exceed the maximum iteration" << std::endl;
			} else {
			
			}
		}
		
		void solve_secant() {
			std::cout << "solve_secant" << std::endl;
			int iter_idx = 0;
			while( iter_idx < cfg.max_iter ) {
				std::cout << "Iter " << iter_idx << std::endl;
				// dx = -[H(x)]-1*dV(x)
				cv::Mat H(cfg.n_var, cfg.n_var, CV_64F);
				for(int i=0; i<cfg.n_var; i++) {
					double out_pre, out;
					(*cfg.V)(cfg.a_pre, out_pre);
					(*cfg.V)(cfg.a, out_pre);
					H.at<double>(i, i) = (out - out_pre) / (cfg.a[i] - cfg.a_pre[i]);
				}
				cv::Mat H_inv = H.inv(cv::DECOMP_SVD);
				cv::Mat J;
				cfg.V->dv(cfg.a, J);
				std::vector<double> d(cfg.n_var, 0.0);
				double val;
				(*cfg.V)(cfg.a, val);
				double d2 = 0.0;
				for(int i=0; i<cfg.n_var; i++) {
					for(int j=0; j<cfg.n_var; j++) {
						d[i] += -(H_inv.at<double>(j, i)*J.at<double>(j));
						d2 += d[i]*d[i];
					}
				}
				// check norm(x)
				if( d2 < cfg.tol ) {
					std::cout << "\nConverged" << std::endl;
					break;
				}
				std::cout << " a = [";
				for(int i=0; i<cfg.n_var; i++) {
					cfg.a[i] += d[i];
					std::cout << cfg.a[i] << " ";
				}
				double val2;
				(*cfg.V)(cfg.a, val2);
				if( val2 - val > 1000.0 ) {
					std::cout << "increase?" << std::endl;
					break;
				}
				iter_idx += 1;
			}
			
			if( iter_idx == cfg.max_iter ) {
				// exceed the maximum iteration
				std::cout << "exceed the maximum iteration" << std::endl;
			} else {
			
			}
		}
		
	public:
		solver(lsq_config _cfg);
		~solver();
		
		void solve() {
			switch(cfg.m) {
				case newton:
					solve_newton();
					break;
				case secant:
					solve_secant();
					break;
				default:
					std::cout << "Unsupport method" << std::endl;
					break;
			}
		}
		
	};
}

#endif //ECE4960_SP18_LSQ_H
