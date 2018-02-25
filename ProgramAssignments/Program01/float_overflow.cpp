//
// Created by yj374 on 2/14/2018.
//

#include <iostream>
#include <utils/output.h>
#include <numeric/floats.h>
#include "program01.h"

void float_overflow() {
    
    l2 << "\n=== float_overflow TEST ===\n"\
         "Use factorial to generate overflow with float\n";
         
    bool output_l1 = false;
    float v = 1.0;
    for(int j=1; j<40; j++) {
        v *= (float)(j);
        l2 << j << "\t" << float2hexstr(v) << " " << is_inf(v) << " " << v << std::endl;
        if( is_inf(v) && !output_l1) {
            l1 << "3. float overflow while j=" << j << "\tv=" << v << "\t is_inf=" << is_inf(v) << std::endl;
            output_l1 = true;
        }
    }
}
