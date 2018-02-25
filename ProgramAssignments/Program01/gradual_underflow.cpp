//
// Created by yj374 on 2/14/2018.
//

#include <iostream>
#include <cmath>
#include <utils/output.h>
#include <numeric/floats.h>
#include "program01.h"

void test_equal(float x, float y) {
	l2 << "x: 0x" << float2hexstr(x) << "\t" << x << std::endl;
	l2 << "y: 0x" << float2hexstr(y) << "\t" << y << std::endl;
	V(l2,(x-y==0));
	V(l2,(x==y));
	V(l2,((x-y)+y==x));
	V(l2,((x/y)==1.0f));
}

void gradual_underflow() {
	
	l2 << "\n=== gradual_underflow TEST ===\n"\
    "v divided by 10 each time\n"
	;
	
	float_union fu_x;
  fu_x.i = 0x00000001;
  float_union fu_y;
  fu_y.i = 0x00000002;
  test_equal(fu_x.f, fu_y.f);
  test_equal(1.4013e-44, 1.4014e-44);

  fu_x.f = 1.0f;
  for(int i=0; i<50; i++) {
    fu_x.f /= 10.0f;
    l2 << i << "\t" << float2hexstr(fu_x.f) << "\t" << fu_x.f << std::endl;
    V(l2, (sin(1.123456789012345f*fu_x.f) / fu_x.f) );
	  if ( i == 41) {
		  l1 << "7. underflow happens while i=" << i << ", " << "sin(1.1234567*f)/f = "
		     << (sin(1.123456789012345f*fu_x.f) / fu_x.f) << std::endl;
	  }
  }
}
