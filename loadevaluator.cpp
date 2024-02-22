#include "loadevaluator.h"
#include "baremetal.h"
#include "host.h"
#include "cloud.h"

vector<BareMetal*> *LoadEvaluator::overLoaded() {
  return UtilizationRateLoadPolice::over();
}

vector<BareMetal*> *LoadEvaluator::underLoaded() {
  return UtilizationRateLoadPolice::under();
}

vector<BareMetal*> *UtilizationRateLoadPolice::over() {
  vector<BareMetal*> *nodes = new vector<BareMetal*>(Cloud::getNodesM().size());
  auto it = Cloud::getNodesM().begin();
  for( int i = 0; it != Cloud::getNodesM().end() ; ++it, ++i ) {
    (*nodes)[i] = *((*it).second->begin());
    auto jt = (*it).second->begin() + 1;
    for( ; jt != (*it).second->end() ; ++jt )
      if( (*jt)->getUtilizationRate() < (*nodes)[i]->getUtilizationRate() )
	(*nodes)[i] = (*jt);
  }
  return nodes;
}

vector<BareMetal*> *UtilizationRateLoadPolice::under() {
  return (vector<BareMetal*> *)NULL;
}
