add_executable(Hw04_PartI PartI.cpp)
add_executable(Hw04_PartII PartII.cpp ../../utils/mem_usage.cpp ../../utils/timer_util.cpp)
target_link_libraries(Hw04_PartII psapi)