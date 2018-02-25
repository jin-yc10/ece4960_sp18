#include <iostream>
#include "program01.h"

void divided_by_zero() {
    
    l2 << "\n=== divided_by_zero TEST ===\n"\
          "CAUSE SIGFPE on WINDOWS, MinGW"\
          "Compiler gave warning: division by zero [-Wdiv-by-zero]\n";
    
    double v = 1.0;
    double double_inf = v / 0.0;
    l1 << "2. 1.0/0.0 = " << double_inf << std::endl;
    l2 << "1.0/0.0 = " << double_inf << std::endl;

    int v_int = 1;
//    int cause_exception = v_int / 0;
    // CAUSE SIGFPE on WINDOWS, MinGW
    // Compiler gave "warning: division by zero [-Wdiv-by-zero]"
}