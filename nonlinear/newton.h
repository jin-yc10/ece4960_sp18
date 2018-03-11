//
// Created by 金宇超 on 3/11/18.
//

#ifndef ECE4960_SP18_NEWTON_H
#define ECE4960_SP18_NEWTON_H

#include "bisection.h"

/// run newton raphson test case
/// \param f function f, should be double f(double x) {}
/// \param df function df, same as f
/// \param init_x init guess of x
/// \param eps epsilon value
/// \return root x for function f
double newton_raphson(equation f, equation df,
                    double init_x, double eps = 1e-8);

/// newton raphson with line search
/// \param f
/// \param df
/// \param init_x
/// \param max_step maximum step length
/// \param eps eplison
double newton_raphson_line_search(equation f, equation df,
                                double init_x, double max_step, double eps = 1e-8);


#endif //ECE4960_SP18_NEWTON_H
