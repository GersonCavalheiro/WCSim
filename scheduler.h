#ifndef __scheduler_h_
#define __scheduler_h_

#include <vector>
#include "user.h"
#include "instance.h"
#include "host.h"
#include "virtualmachine.h"
#include "task.h"

struct Scheduler {
  static inline void localSchedule() {
    static int lastDataStamp = 0;
    if( lastDataStamp == GlobalClock::get() ) return;
    lastDataStamp = GlobalClock::get();
    auto it = Instance::getInstancesL().begin();
    for( ; it != Instance::getInstancesL().end() ; ++it )
      if( (*it)->getStatus() == alive )
	 (*it)->localSchedule();
  }
  static void  nodeScheduler();
  static void  cloudScheduler();
  static void  burstScheduler();
  static VM*   vmSelection( User& owner, Task& task );
  static Host* hostSelection( Node& node, Instance& vm );
  static Host* receiverNodeSelection( Host& sender );
  static Host* receiverCloudSelection( Host& sender );
};

struct VMSelection {
  static VM* fixed( User& owner, Task& task );
  static VM* random( User& owner, Task& task );
  static VM* circular( User& owner, Task& task );
  static VM* load( User& owner, Task& task );
  static VM* rate( User& owner, Task& task );
  static VM* selectVMByNbInstructions( User& owner, Task& task );
};

struct HostSelection {
  static Host* fixed( Node& node, Instance& vm );
  static Host* random( Node& node, Instance& vm );
  static Host* circular( Node& node, Instance& vm );
  static Host* rate( Node& node, Instance& vm );
  static Host* bestFit( Node& node, Instance& vm );
  static Host* worstFit( Node& node, Instance& vm );
};

struct VMMigrationHostSelection {
  static Host* randomNodeReceiver( Host& sender );
  static Host* circularNodeReceiver( Host& sender );
  static Host* randomCloudReceiver( Host& sender );
  static Host* circularCloudReceiver( Host& sender );
};

struct VMMigrationVMSelection {
  static VM* random( Host& sender );
};


#endif
