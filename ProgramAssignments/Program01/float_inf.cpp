//
// Created by yj374 on 2/14/2018.
//

#include <utils/output.h>
#include <limits>
#include <iostream>
#include <cmath>
#include "program01.h"

float one_divided_by_x(float x) {
    return 1/x;
}

float sin_x(float x) {
    return std::sin(x);
}

float exp_x(float x) {
    return std::exp(x);
}

void float_inf() {
    
    l2 << "\n=== float_inf TEST ===\n"\
         "Use std::numeric_limits<float>::infinity() to get inf\n";
    
    float float_inf = std::numeric_limits<float>::infinity();
    float float_neg_inf = -float_inf;
    l2 << "float_inf:" << float2hexstr(float_inf) << " " << float_inf << std::endl;
    l2 << "float_neg_inf:" << float2hexstr(float_neg_inf) << " " << float_neg_inf << std::endl;

    l2 << "1/inf = " << one_divided_by_x(float_inf) << std::endl;           // 0
    l2 << "sin(inf) = " << sin_x(float_inf) << std::endl;                 // nan
    l2 << "exp(inf) = " << exp_x(float_inf) << std::endl;                 // inf

    l2 << "1/ninf = " << one_divided_by_x(float_neg_inf) << std::endl;      // -0
    l2 << "sin(ninf) = " << sin_x(float_neg_inf) << std::endl;             // nan
    l2 << "exp(ninf) = " << exp_x(float_neg_inf) << std::endl;             // 0

    l2 << "inf+ninf = " << float_inf + float_neg_inf << std::endl;        // nan
    l2 << "0.0*inf = " << 0.0f*float_inf << std::endl;                   // nan
    l2 << "0.0*ninf = " << 0.0f*float_neg_inf << std::endl;               // nan
}
