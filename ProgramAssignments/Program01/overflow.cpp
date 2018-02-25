#include <stdio.h>
#include "numeric/int.h"
#include "program01.h"

void overflow() {

  l2 << "\n=== OVERFLOW TEST ===\n"\
         "Use factorial to generate overflow with integer\n"\
         "The overflow is detected by both \'__builtin_smul_overflow\' and \'mul_overflow_alt\'\n"\
         "The * at the beginning of the line indicate whether an overflow occurs.\n"\
         "The first * is for mul_overflow_alt and the second * is __builtin_smul_overflow\n\n";

  int i=1;
  int r;
  bool output_l1 = false;
  for(int j=1; j<15; j++) {
    if( mul_overflow_alt(i,j,r) ) {
      l2 << j << (" ");
    } else {
      l2 << j << ("*");
      if ( !output_l1 ) {
        l1 << "1. overflow happens while j=" << j << " i=" << i << std::endl;
        output_l1 = true;
      }
    }
    if(__builtin_smul_overflow(i,j,&r)) {
      l2 << "* " << r << std::endl;
    } else {
      l2 << "  " << r << std::endl;
    }
    i *= j;
  }
}