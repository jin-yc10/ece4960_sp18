//
// Created by yj374 on 2/25/2018.
//

#ifndef ECE4960_SP18_TIMER_UTIL_H
#define ECE4960_SP18_TIMER_UTIL_H

class timer_util {
public:
  void tick();                              // start measuring time
  void tock(const char* prefix=0) const;    // done measuring time; print elapsed seconds, optionally prefixed with 'prefix'
  double silent_tock() const;               // done measuring time; give elapsed seconds
private:
  double tick_; // stores starting point in seconds
};


#endif //ECE4960_SP18_TIMER_UTIL_H
