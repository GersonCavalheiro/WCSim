#ifndef __simulator_h__
#define __simulator_h__

#include <list>
#include "utils.h"
#include "event.h"
#include "globalclock.h"

using namespace std;

class Simulator {
  static int          eventCount;
  static list<Event*> eventL;
  static inline void newEvent( Event *ev ) { orderInsert(eventL,ev); }
public:
  static int lastBoTFinishDate;
  static int lastTaskFinishDate;
  Simulator() {}
  static inline Event* nextEvent() {
    if( eventL.size() == 0 ) return NULL;
    auto aux = eventL.front();
    eventL.pop_front();
    return aux;
  }
  static void run();
  static void printAllEvents();
  friend class Event;
};

#endif
