//
// Created by yj374 on 2/25/2018.
//

#include "timer_util.h"

#include <chrono>
#include <iostream>
#include <iomanip>

const static std::chrono::steady_clock::time_point reference_time = std::chrono::steady_clock::now();

static void print(const char* prefix, const double dt) {
  // helper function to print elapsed time
  if (prefix!=0)
    std::cout << prefix << "\t" << std::setprecision(4) << dt << " sec\n";
  else
    std::cout << "    \t" << std::setprecision(4) << dt << " sec\n";
}

static double elapsed_time() {
  std::chrono::steady_clock::time_point this_time = std::chrono::steady_clock::now();
  double dt = std::chrono::duration<double>(this_time - reference_time).count();
  return dt;
}

void timer_util::tick() {
  // start measuring time
  tick_ = elapsed_time();
}

void timer_util::tock(const char* prefix) const {
  // done measuring time; print elapsed seconds
  print(prefix, silent_tock());
}

double timer_util::silent_tock() const {
  // done measuring time; give elapsed seconds
  double tock = elapsed_time();
  double dt = tock-tick_;
  return dt;
}