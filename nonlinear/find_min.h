#ifndef _FIND_MIN_H_
#define _FIND_MIN_H_

#include <vector>
#include <general/functor.h>

// class to solve nonliniear optimization problem
namespace nonlinear {

	enum find_min_method {
		FIND_MIN_NEWTON,
		FIND_MIN_QUASI_NEWTON,
		FIND_MIN_QUASI_NEWTON_SECANT,
		FIND_MIN_LM
	};

	struct find_min_config {
		int verbose;
		int max_iter;
		int step;
		int n_var;
		double tol;
		std::vector<double> x;
		cv::Mat J, H;
		find_min_method method;
		general::functor* func;
	};

	class find_min {
		find_min_config cfg;
	public:
		find_min(find_min_config cfg_);
		~find_min() {}

		void solve();
		void solve_newton();
		void solve_quasi_newton();
		void solve_lm();
	};
}; // namespace nonlinear



#endif //_FIND_MIN_H_