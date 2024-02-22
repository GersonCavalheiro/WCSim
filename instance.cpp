#include <vector>

#include "instance.h"
#include "host.h"
#include "node.h"
#include "scheduler.h"
#include "task.h"
#include "user.h"

vector<Instance*> Instance::instanceL;
int Instance::nbInstances = 0;

// SCHEDULE
//         VM PLACEMENT SELECTION
Instance::Instance( Node *n, User *u, int vCores, int vMips, int vRam)
          : sourceNode(n), owner(u), vCores(vCores), vMips(vMips),
	    vRam(vRam), nbTasks(0) {
  id = nbInstances++;
  instanceL.push_back(this);
  running = Scheduler::hostSelection(*(owner->getNode()),*this);
  running->pushVM(this);
  setStatus(alive);
}

void Instance::goHome() {
  this->migrate(sourceNode->getId());
}

void Instance::place( Task *t ) {
  occupedVRam += t->getRam();
  if( nbTasks < vCores ) running->pinCore();
  ++nbTasks;
}

void Instance::unplace( Task *t ) {
  occupedVRam -= t->getRam();
  if( nbTasks <= vCores ) running->releaseCore();
  --nbTasks;
}

void Instance::avanceTask( Task *t, int nbTasks ) {
  if( GlobalClock::get() == t->getDataStamp() ) return;
  if( getStatus() != alive ) abort();

  int actualMips = (running->getActualMips() < this->getVMips())
                   ? running->getActualMips() 
                   : (int)((float)(this->getVMips()) * running->getUtilizationRate());
  actualMips = (nbTasks<=vCores)?actualMips:(int)((float)((actualMips*vCores))/nbTasks);
  if( actualMips == 0 ) actualMips = 1; //Avoiding below 1 mpis
  int executed = actualMips
	         * (GlobalClock::get() - t->getDataStamp());
  t->hup(executed);
  this->getOwner()->billing(running->getId(),executed);
}

bool Instance::fitRam( Task *t ) {
  return (vRam-occupedVRam) >= t->getRam();
}

int Instance::getRunningHostId() {
  return running->getId();
}

