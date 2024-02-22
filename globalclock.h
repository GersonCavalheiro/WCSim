#ifndef __global_clock_h__
#define __global_clock_h__

#include <stdlib.h>

class GlobalClock {
private:
  static int clock;
public:
  GlobalClock() {}
  static inline void set( int t ) { if(clock < t) clock = t; } //else abort(); }
  static inline int get() { return clock; }
};
#endif
