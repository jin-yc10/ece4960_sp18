#include "find_min.h"

namespace nonlinear {
  find_min::find_min( find_min_config _cfg ) : cfg(_cfg) {

  }

  void find_min::solve() {
    switch(cfg.method) {
      case FIND_MIN_QUASI_NEWTON:
        solve_quasi_newton();
        break;
    }
  }

  void find_min::solve_quasi_newton() {
    std::cout << "find_min::solve_quasi_newton" << std::endl;
	int step = 0;
	std::cout << "initial guess x = [";
	for (auto x0 : cfg.x) {
		std::cout << x0 << " ";
	}
	std::cout << "]" << std::endl;
	while (step < cfg.max_iter) {
		cv::Mat J, H;
		(*cfg.func).dv(cfg.x, J);
		(*cfg.func).dv2(cfg.x, H);
		std::cout << J << std::endl;
		std::cout << H << std::endl;
		cv::Mat H_inv = H.inv(cv::DECOMP_SVD);
		std::vector<double> d(cfg.n_var, 0.0);
		double d2 = 0.0;
		for (int i = 0; i<cfg.n_var; i++) {
			for (int j = 0; j<cfg.n_var; j++) {
				d[i] += -(H_inv.at<double>(j, i)*J.at<double>(j));
				d2 += d[i] * d[i];
			}
		}
		//d2 = std::sqrt(d2);
		if (cfg.verbose) {
			double val;
			(*cfg.func)(cfg.x, val);
			std::cout << "Iter " << step;
			std::cout << " | F(x) = " << val;
			std::cout << " ||d||2 = " << d2;
			std::cout << ", x = [";
			for (int i = 0; i<cfg.n_var; i++) {
				std::cout << cfg.x[i] << " ";
			}
			std::cout << "]" << "\n";
		}

		if (d2 < cfg.tol) {
			std::cout << "\nConverged" << std::endl;
			break;
		}
		for (int i = 0; i<cfg.n_var; i++) {
			cfg.x[i] += d[i];
		}

		
		step++;
	}
  }


}; // namespace nonlinear