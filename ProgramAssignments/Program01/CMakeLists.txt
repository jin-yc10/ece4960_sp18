include_directories(../..)

set(CASES overflow
        divided_by_zero
        float_overflow
        float_inf
        float_nan
        signed_zero
        gradual_underflow
        calc_pi
)

set(PROGRAM01_SRCS "")
#
foreach(C ${CASES})
    message(STATUS ${C})
    set(PROGRAM01_SRCS ${PROGRAM01_SRCS} ${C}.cpp)
endforeach()

add_executable(program01 program01.cpp ${PROGRAM01_SRCS}
        ../../utils/output.cpp
        ../../numeric/floats.cpp)


