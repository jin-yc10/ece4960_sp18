//
// Created by yj374 on 2/4/2018.
//

#ifndef ECE4960_SP18_OUTPUT_H
#define ECE4960_SP18_OUTPUT_H

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

#endif //ECE4960_SP18_OUTPUT_H
