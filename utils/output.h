//
// Created by yj374 on 2/4/2018.
//

#ifndef ECE4960_SP18_OUTPUT_H
#define ECE4960_SP18_OUTPUT_H

#include <string>

#define INFO 0
#define WARN 1
#define CRIT 2

#include <fstream>

#define OUTPUT_LEVEL INFO

#define V(s, X) s << #X << " = " << X << std::endl
#define O(s, X) s std::cout << X << std::endl

#define output_to(stream, x) stream << X << std::endl

std::string double2hexstr(double x);
std::string float2hexstr(float x);

#endif //ECE4960_SP18_OUTPUT_H
