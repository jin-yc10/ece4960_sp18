//
// Created by yj374 on 2/16/2018.
//

#include "output.h"


std::string double2hexstr(double x) {
  union {
    long long i;
    double    d;
  } value;

  value.d = x;
  char buf[17];
  snprintf (buf,sizeof(buf),"%016llx",value.i);
  buf[16]=0; //make sure it is null terminated.
  return std::string(buf);
}

std::string float2hexstr(float x) {
  union {
    unsigned int   i;
    float f;
  } value;

  value.f = x;
  char buf[12] = {0};
  snprintf (buf,sizeof(buf),"%08x",value.i);
  buf[9]=0; //make sure it is null terminated.
  return std::string(buf);
}