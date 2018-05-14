#include <general/functor.h>
#include <nonlinear/find_min.h>

#include <dlib/optimization.h>
#include <dlib/global_optimization.h>

using namespace dlib;
// V = (3x12 + x2 - 4)2 + (x12 - 3x2 + 2)2

class rosen2 : public general::functor {
public:
	rosen2(int n_in) :general::functor(n_in) {}
	void set_params(std::vector<double> params_) {
		this->params = params_;
	}
	void operator()(std::vector<double> in, double& out) {
		const double x = in[0];
		const double y = in[1];

		// compute Rosenbrock's function and return the result
		out = 100.0*std::pow(y - x*x, 2) + std::pow(1 - x, 2);
	}

	void dv_(std::vector<double> in, cv::Mat& J_out) {
		double x = in[0];
		double y = in[1];
		J_out = cv::Mat(n_in, 1, CV_64F);
		J_out.at<double>(0, 0) = -400 * x*(y - x*x) - 2 * (1 - x); // derivative of rosen() with respect to x
		J_out.at<double>(1, 0) = 200 * (y - x*x);              // derivative of rosen() with respect to y
	};

	void dv2_(std::vector<double> in, cv::Mat& H_out) {
		H_out = cv::Mat(n_in, n_in, CV_64F);
		double x = in[0];
		double y = in[1];

		// now compute the second derivatives 
		H_out.at<double>(0, 0) = 1200 * x*x - 400 * y + 2; // second derivative with respect to x
		H_out.at<double>(0, 1) = -400 * x;
		H_out.at<double>(1, 0) = H_out.at<double>(0, 1);// derivative with respect to x and y
		H_out.at<double>(1, 1) = 200;                 // second derivative with respect to y
	};
};

typedef matrix<double, 0, 1> column_vector;


double rosen(const column_vector& m)
/*
This function computes what is known as Rosenbrock's function.  It is
a function of two input variables and has a global minimum at (1,1).
So when we use this function to test out the optimization algorithms
we will see that the minimum found is indeed at the point (1,1).
*/
{
	const double x = m(0);
	const double y = m(1);

	// compute Rosenbrock's function and return the result
	return 100.0*pow(y - x*x, 2) + pow(1 - x, 2);
}

// This is a helper function used while optimizing the rosen() function.  
const column_vector rosen_derivative(const column_vector& m)
/*!
ensures
- returns the gradient vector for the rosen function
!*/
{
	const double x = m(0);
	const double y = m(1);

	// make us a column vector of length 2
	column_vector res(2);

	// now compute the gradient vector
	res(0) = -400 * x*(y - x*x) - 2 * (1 - x); // derivative of rosen() with respect to x
	res(1) = 200 * (y - x*x);              // derivative of rosen() with respect to y
	return res;
}

// This function computes the Hessian matrix for the rosen() fuction.  This is
// the matrix of second derivatives.
matrix<double> rosen_hessian(const column_vector& m)
{
	const double x = m(0);
	const double y = m(1);

	matrix<double> res(2, 2);

	// now compute the second derivatives 
	res(0, 0) = 1200 * x*x - 400 * y + 2; // second derivative with respect to x
	res(1, 0) = res(0, 1) = -400 * x;   // derivative with respect to x and y
	res(1, 1) = 200;                 // second derivative with respect to y
	return res;
}

// ----------------------------------------------------------------------------------------

class rosen_model
{
	/*!
	This object is a "function model" which can be used with the
	find_min_trust_region() routine.
	!*/

public:
	typedef ::column_vector column_vector;
	typedef matrix<double> general_matrix;

	double operator() (
		const column_vector& x
		) const {
		return rosen(x);
	}

	void get_derivative_and_hessian(
		const column_vector& x,
		column_vector& der,
		general_matrix& hess
	) const
	{
		der = rosen_derivative(x);
		hess = rosen_hessian(x);
	}
};

int main() {
  nonlinear::find_min_config cfg;
  cfg.method = nonlinear::FIND_MIN_QUASI_NEWTON;
  cfg.max_iter = 1e3;
  cfg.tol = 1e-7;
  cfg.n_var = 2;
  rosen2* v = new rosen2(2);
  cfg.func = v;
  cfg.x = { 4, 8 };
  cfg.verbose = 1;
  nonlinear::find_min fm(cfg);
  fm.solve();

  column_vector starting_point = { 4, 8 };
  find_min_trust_region(objective_delta_stop_strategy(1e-7).be_verbose(),
	  rosen_model(),
	  starting_point,
	  10 // initial trust region radius
  );
  std::cout << "rosen solution: \n" << starting_point << std::endl;

  starting_point = { 0.8, 1.3 };
  find_min(bfgs_search_strategy(),  // Use BFGS search algorithm
	  objective_delta_stop_strategy(1e-7).be_verbose(), // Stop when the change in rosen() is less than 1e-7
	  rosen, derivative(rosen), starting_point, -1);
  // Once the function ends the starting_point vector will contain the optimum point 
  // of (1,1).
  std::cout << "rosen solution:\n" << starting_point << std::endl;

  return 0;
}