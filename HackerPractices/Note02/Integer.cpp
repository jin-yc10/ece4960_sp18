//
// Created by yj374 on 2/4/2018.
//

#include <cmath>
#include <iostream>

int main(int argc, char** argv) {
    long i = 1, intFactorial = 1;

    for (i= 2; i < 20; i++) {
        intFactorial *= i;
        printf("i=%d, intFactorial=%d\n", i, intFactorial);
    }

    for (i= 20; i > 1; i--) {
        intFactorial /= i;
        printf("i=%d, intFactorial=%d\n", i, intFactorial);
    }
/*
i=2, intFactorial=2
i=3, intFactorial=6
i=4, intFactorial=24
i=5, intFactorial=120
i=6, intFactorial=720
i=7, intFactorial=5040
i=8, intFactorial=40320
i=9, intFactorial=362880
i=10, intFactorial=3628800
i=11, intFactorial=39916800
i=12, intFactorial=479001600
i=13, intFactorial=1932053504
i=14, intFactorial=1278945280
i=15, intFactorial=2004310016
i=16, intFactorial=2004189184
i=17, intFactorial=-288522240  <== overflow!
i=18, intFactorial=-898433024
i=19, intFactorial=109641728
i=20, intFactorial=5482086
i=19, intFactorial=288530
i=18, intFactorial=16029
i=17, intFactorial=942
i=16, intFactorial=58
i=15, intFactorial=3
i=14, intFactorial=0
i=13, intFactorial=0
i=12, intFactorial=0
i=11, intFactorial=0
i=10, intFactorial=0
i=9, intFactorial=0
i=8, intFactorial=0
i=7, intFactorial=0
i=6, intFactorial=0
i=5, intFactorial=0
i=4, intFactorial=0
i=3, intFactorial=0
i=2, intFactorial=0
 */
}