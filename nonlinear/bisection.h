//
// Created by 金宇超 on 3/11/18.
//

#ifndef ECE4960_SP18_BISECTION_H
#define ECE4960_SP18_BISECTION_H

#include <vector>

#define DEBUG_INFO(x) x
//#define DEBUG_INFO(x) {}

struct range {
	double lb;
	double ub;
};

typedef double (*equation)(double);
typedef double (*equation_2d)(double, double);

///
/// \param f
/// \param lb_
/// \param ub_
/// \param eps_
/// \param out reference double, to get the output x
/// \param output_iteration
/// \return
bool bisection_1d(equation f, double lb_, double ub_,
                    double& out, double eps_ = 1e-8, bool output_iteration = false);

///
/// \param f
/// \param rx
/// \param ry
/// \return
bool is_sign_change(equation_2d f, range rx, range ry);

///
/// \param v
/// \return
double mid_value(range v);

/// bisection_2d use 'candidates' to simulate recusive function. This is to get ride of call-stack overflow.
/// \param init_rx
/// \param init_ry
/// \param f1
/// \param f2
/// \param eps
/// \return pair of double (x, y) or (nan, nan) if no answer
std::pair<double, double> bisection_2d(range init_rx, range init_ry,
                  equation_2d f1, equation_2d f2, double eps);

#endif //ECE4960_SP18_BISECTION_H
