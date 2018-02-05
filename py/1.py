from __future__ import print_function
import math
import sys

print(sys.float_info)

def quad(x, a, b, c):
    return a*x*x+b*x+c

a = 1
b = 1.0e23
c = (1.0e23)-1
print(b-c)
print(b*b-4*a*c)
d = math.sqrt(b*b-4*a*c)
print(d*d - (b*b-4*a*c))
print(b-math.sqrt(b*b-4*a*c))
print(quad(c, a, b, c))

def run_test(func, a_, b_, c_):
    print(func.__name__)
    x1, x2 = func(a_, b_, c_)
    print(x1, x2, "\t", quad(x1, a_, b_, c_), quad(x2, a_, b_, c_))

def naive(a_,b_,c_):
    x1 = (-b + math.sqrt(b * b - 4 * a * c)) / (2 * a)
    x2 = (-b - math.sqrt(b * b - 4 * a * c)) / (2 * a)
    return x1, x2

def alt(a_, b_, c_):
    dv = (-b + math.sqrt(b * b - 4 * a * c))
    if dv != 0:
        x1 = (-2.0 * c) / dv
    else:
        x1 = -1.0
    x2 = (-2.0 * c) / (-b - math.sqrt(b * b - 4 * a * c))
    return x1, x2

def alt2(a_, b_, c_):
    b_2a = (-b/(2*a))
    x1 = b_2a + math.sqrt(b_2a * b_2a - c)
    x2 = b_2a - math.sqrt(b_2a * b_2a - c)
    return x1, x2

run_test(naive, a, b, c)
run_test(alt, a, b, c)
run_test(alt2, a, b, c)