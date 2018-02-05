//
// Created by yj374 on 2/4/2018.
//

#include <cmath>
#include <iostream>

int main(int argc, char** argv) {
    double x = 0.0, y = 0.0;
    double doubleResult1, doubleResult2;
    doubleResult1 = 1/x;
    doubleResult2 = y/x;
    std::cout << doubleResult1 << "\t" << doubleResult2 << std::endl;
    // inf  nan

    long m = 0, n = 0, intResult1, intResult2;
    intResult1 = 1/m;  intResult2 = m/n;
    // cause Arithmetic exception on mingw
    std::cout << intResult1 << "\t" << intResult2 << std::endl;
}