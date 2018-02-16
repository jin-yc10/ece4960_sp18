//
// Created by yj374 on 2/16/2018.
//

#include "floats.h"

bool is_inf(float f) {
  float_union fu;
  fu.f = f;
  return fu.i == 0x7f800000;
}

bool is_ninf(float f) {
  float_union fu;
  fu.f = f;
  return fu.i == 0xff800000;
}