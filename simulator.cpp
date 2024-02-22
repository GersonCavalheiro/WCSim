#include <iostream>
#include <list>

#include "simulator.h"
#include "host.h"
#include "node.h"
#include "cloud.h"
#include "event.h"
#include "globalclock.h"
#include "loadevaluator.h"
#include "scheduler.h"

using namespace std;

int          Simulator::eventCount = 0;
list<Event*> Simulator::eventL;
int          Simulator::lastBoTFinishDate = 0; //Updated by BoTFinishEv::execute
int          Simulator::lastTaskFinishDate = 0; //Updated by TaskFinishEv::execute
void Simulator::printAllEvents() {
  int i = 0;
  for( auto it = eventL.begin() ; it != eventL.end() ; ++it, ++i )
    cout << "[" << i << "]" << *(*it) << endl;
}

void Simulator::run() {
  Event *nEv;
  int nbEvents = 0;

  GlobalClock::set(eventL.front()->getDate());
  GlobalClock::set(0);

  while( !eventL.empty() || BoT::undoneBoTs() > 0 ) {
    if( !eventL.empty() ) {
      Event* ev = eventL.front();
      if( ev->getDate() == GlobalClock::get() ) {
        eventL.pop_front();
        //cout << *ev << endl;
        ev->execute();
       delete(ev);
       ++nbEvents;
      } else GlobalClock::set(GlobalClock::get()+1);
    }
    Scheduler::localSchedule();
    if( BoT::undoneBoTs() == 0 ) return;
//    if( !(GlobalClock::get()%100)) cout << "Size = " << eventL.size() << " [" << GlobalClock::get() << ":" << nbEvents << "]" << endl;
    if( !(GlobalClock::get()%300) ) {
      cout << "[" << GlobalClock::get() << "]: " << BoT::undoneBoTs() << " " << Cloud::uncompletedTasks() << " " << eventL.size() << endl;
#ifdef NODESCH
      Scheduler::nodeScheduler();
#endif
#ifdef CLOUDSCH
      Scheduler::cloudScheduler();
#endif
#ifdef BURSTSCH
      Scheduler::burstScheduler();
#endif
    }
  }

  cout << "Uncompleted tasks: " << Cloud::uncompletedTasks() << endl;
}
