//
// Created by yj374 on 2/16/2018.
//

#include "program01.h"
#include "utils/output.h"

std::ofstream l1 = std::ofstream("level1.txt");
std::ofstream l2 = std::ofstream("level2.txt");

int main(int argc, char** argv) {
  overflow();
  divided_by_zero();
  float_overflow();
  float_inf();
  float_nan();
  signed_zero();
  gradual_underflow();

  calc_pi();
}