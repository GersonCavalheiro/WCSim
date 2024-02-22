#ifndef _loadevaluator_h__
#define _loadevaluator_h__

#include "baremetal.h"
#include "host.h"

struct LoadEvaluator {
  static vector<BareMetal*> *overLoaded();
  static vector<BareMetal*> *underLoaded();
};

struct UtilizationRateLoadPolice {
   static vector<BareMetal*> *over();
   static vector<BareMetal*> *under();
};
#endif
