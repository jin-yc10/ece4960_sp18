
#include <dlib/optimization.h>
#include <iostream>
#include <vector>


using namespace std;
using namespace dlib;

// ----------------------------------------------------------------------------------------

typedef matrix<double,2,1> input_vector;
typedef matrix<double,3,1> parameter_vector;

// ----------------------------------------------------------------------------------------

const double Vsb = 0.0;       //S is shorted to B
const double VT = 26E-3;      //VT in volts

double id_ekv(double Vgb, double Vdb, double Is, double K, double Vth) {
	double l1 = std::log(1+std::exp((K*(Vgb - Vth)-Vsb)/(2.0*VT))); // If
	double l2 = std::log(1+std::exp((K*(Vgb - Vth)-Vdb)/(2.0*VT))); // Ir
	return Is * (l1 * l1 - l2 * l2);
} // Id

double model (
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

double residual (
    const std::pair<input_vector, double>& data,
    const parameter_vector& params
) {
    return model(data.first, params) - data.second;
}


int main()
{
  // Now let's generate a bunch of input/output pairs according to our model.
  std::vector<std::pair<input_vector, double> > data_samples;
  input_vector input;
  std::ifstream nmos("C:/Dev/ece4960_sp18/data/outputNMOS.txt");
	std::string nothing;
  int n_samples = 1010;
	for(int i=0; i<n_samples; i++) {
		double v1, v2, v3;
		nmos >> v1 >> v2 >> v3;
    input = randm(2, 1);
    input(0) = v1; input(1) = v2;
    const double output = v3;
    data_samples.push_back(make_pair(input, output));
	}
	nmos.close();
  parameter_vector x;
  x = {1e-7, 1, 1};

  //derivative(residual)(data_samples[0], params)

  cout << "Use Levenberg-Marquardt" << endl;
  // Use the Levenberg-Marquardt method to determine the parameters which
  // minimize the sum of all squared residuals.
  solve_least_squares_lm(objective_delta_stop_strategy(1e-7).be_verbose(), 
                          residual,
                          derivative(residual),
                          data_samples,
                          x);

  // Now x contains the solution.  If everything worked it will be equal to params.
  cout << "inferred parameters: "<< x << endl;
  cout << endl;

  x = {1e-7, 1, 1};
  cout << "Use Levenberg-Marquardt, approximate derivatives" << endl;
  // If we didn't create the residual_derivative function then we could
  // have used this method which numerically approximates the derivatives for you.
  solve_least_squares_lm(objective_delta_stop_strategy(1e-7).be_verbose(), 
                          residual,
                          derivative(residual),
                          data_samples,
                          x);

  // Now x contains the solution.  If everything worked it will be equal to params.
  cout << "inferred parameters: " << x << endl;
  cout << endl;

  x = {1e-7, 1, 1};
  cout << "Use Levenberg-Marquardt/quasi-newton hybrid" << endl;
  // This version of the solver uses a method which is appropriate for problems
  // where the residuals don't go to zero at the solution.  So in these cases
  // it may provide a better answer.
  solve_least_squares(objective_delta_stop_strategy(1e-7).be_verbose(), 
                      residual,
                      derivative(residual),
                      data_samples,
                      x);

  // Now x contains the solution.  If everything worked it will be equal to params.
  cout << "inferred parameters: " << x << endl;
  return 0;
}