//
// Created by yj374 on 2/4/2018.
//

#include <cmath>
#include <iostream>

int main(int argc, char** argv) {
    double a = 1e-20;
    double b = 1e3;
    double c = 1e3;
    double sq = sqrt(b*b-4.0*a*c);
    double x1 = (-b+sq)/(2.0*a);
    double x2 = (-b-sq)/(2.0*a);
    std::cout << "x1=" << x1 << ",x2=" << x2 << std::endl;
    // x1=0,x2=-1e+023
    // lose precision while calculate the sqrt
}