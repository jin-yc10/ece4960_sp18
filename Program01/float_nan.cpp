//
// Created by yj374 on 2/14/2018.
//

#include <utils/output.h>
#include <limits>
#include <iostream>
#include <cmath>
#include <cfenv>
#include <numeric/floats.h>
#include "program01.h"

#pragma STDC_FENV_ACCESS on

void show_fe_exceptions() {
    int n = std::fetestexcept(FE_ALL_EXCEPT);
    if(n & FE_INVALID) std::cout << "FE_INVALID is raised\n";
    else if(n == 0)    std::cout << "no exceptions are raised\n";
    std::feclearexcept(FE_ALL_EXCEPT);
}

void float_nan() {
  
  l2 << "\n=== float_nan TEST ===\n"\
    "In std library we have two different kind of nan( quiet, signal) \n"\
    "quiet nan is just nan while signal nan will cause an exception\n"
  ;
  
  
  float float_nan = std::numeric_limits<float>::quiet_NaN();
  l2 << float2hexstr(float_nan) << "\t" << float_nan << std::endl;

  float float_nan_signal = std::numeric_limits<float>::signaling_NaN();
  l2 << float2hexstr(float_nan_signal) << "\t" << float_nan_signal << std::endl;
  
  float_union fu;
  fu.f = float_nan;
  fu.i |= 0x80000000;
  float float_neg_nan = fu.f;
  l2 << float2hexstr(float_neg_nan) << "\t" << float_neg_nan << std::endl;
}
