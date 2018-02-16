//
// Created by yj374 on 2/16/2018.
//

#ifndef ECE4960_SP18_INT_H
#define ECE4960_SP18_INT_H

bool mul_overflow_alt(int i, int j, int& r) {
  r = i*j;
  return  r/j == i;
}

#endif //ECE4960_SP18_INT_H
