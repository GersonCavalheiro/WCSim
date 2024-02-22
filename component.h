#ifndef __component_h__
#define __component_h__

#include <vector>
#include <queue>
#include "event.h"

class Component {
  static int compCount;
  static vector<Component*> compsL;
  queue<Event*> eventsQ;
  int id;
protected:
  char *componentName;
public:
  Component() { id = compCount++; compsL.push_back(this); }
  inline void   pushEvent( Event *ev ) { eventsQ.push(ev); }
  inline void   popEvent() { eventsQ.pop(); }
  inline Event *inCurseEvent() { return eventsQ.front(); }
  inline char  *getComponentName() { return componentName; }
};
#endif
