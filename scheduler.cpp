#include <stdlib.h>
#include <vector>

#include "loadevaluator.h"
#include "scheduler.h"
#include "user.h"
#include "node.h"
#include "host.h"
#include "cloud.h"
#include "virtualmachine.h"
#include "task.h"

void Scheduler::nodeScheduler() {
  vector<BareMetal*> *hosts = LoadEvaluator::overLoaded();
  for( auto it = hosts->begin() ; it != hosts->end() ; ++it ) {
    if( (Host*)(*it)->isPublic() ) continue;
    if( (*it)->getUtilizationRate() <= 0.5 && (*it)->getVMMap().size() > 1 ) {
     auto vm = ((*it)->getVMMap().begin())->second;
     auto receiver = Scheduler::receiverNodeSelection(*(Host*)(*it));
     vm->migrate(receiver);
     new InstanceResumeEv(vm,GlobalClock::get()+2);
    }
  }
}

void Scheduler::cloudScheduler() {
  vector<BareMetal*> *hosts = LoadEvaluator::overLoaded();
  for( auto it = hosts->begin() ; it != hosts->end() ; ++it ) {
    if( (Host*)(*it)->isPublic() ) continue;
    if( (*it)->getUtilizationRate() <= 0.5
        && (*it)->isPublic() == false
	&& (*it)->getVMMap().size() > 1 ) {
      auto vm = ((*it)->getVMMap().begin())->second;
      auto receiver = Scheduler::receiverCloudSelection(*(Host*)(*it));
      vm->migrate(receiver);
      new InstanceResumeEv(vm,GlobalClock::get()+10);
    }
  }
}

void Scheduler::burstScheduler() {
  if( Cloud::getPublicHostsL().size() == 0 ) return;
  vector<BareMetal*> *hosts = LoadEvaluator::overLoaded();
  for( auto it = hosts->begin() ; it != hosts->end() ; ++it ) {
    if( (Host*)(*it)->isPublic() ) continue;
    if( (*it)->getUtilizationRate() <= 0.5
        && (*it)->isPublic() == false
	&& (*it)->getVMMap().size() > 1 ) {
      auto vm = ((*it)->getVMMap().begin())->second;
      auto receiver = Cloud::getPublicHostsL()[rand()%Cloud::getPublicHostsL().size()];
      vm->migrate((Host*)receiver);
      new InstanceResumeEv(vm,GlobalClock::get()+10);
    }
  }
}

VM *Scheduler::vmSelection( User& owner, Task& task ) {
#ifdef RANDOMSCH
  return VMSelection::random(owner,task);
#endif
#ifdef FIXEDSCH
  return VMSelection::fixed(owner,task);
#endif
#ifdef CIRCULARSCH
  return VMSelection::circular(owner,task);
#endif
  abort();
  return (VM*)NULL;
}

Host *Scheduler::hostSelection( Node& node, Instance& vm ) {
#ifdef RANDOMSCH
 return HostSelection::random(node,vm);
#endif
#ifdef FIXEDSCH
 return HostSelection::fixed(node,vm);
#endif
#ifdef CIRCULARSCH
 return HostSelection::circular(node,vm);
#endif
  abort();
  return (Host*)NULL;
}

Host *Scheduler::receiverNodeSelection( Host& sender ) {
  return VMMigrationHostSelection::randomNodeReceiver(sender);
}

Host *Scheduler::receiverCloudSelection( Host& sender ) {
  return VMMigrationHostSelection::randomNodeReceiver(sender);
}

// ------------------------

VM* VMSelection::fixed( User& owner, Task& task ) {
  return *(owner.getVMList()).begin();
  /*
  vector<VM*>& vmL = owner.getVMList();
  return *vmL.begin();*/
}

VM* VMSelection::random( User& owner, Task& task ) {
  vector<VM*>& vmL = owner.getVMList();
  return vmL[rand()%vmL.size()];
}

VM* VMSelection::circular( User& owner, Task& task ) {
  static int p = 0;
  VM *choice = owner.getVMList()[p];

  ++p;
  p = (p == owner.getVMList().size()) ? 0 : p;

  return choice;
}

VM* VMSelection::load( User& owner, Task& task ) {
  vector<VM*>& vmL = owner.getVMList();
  auto it = owner.getVMList().begin();
  VM*  vm = *it;
  for( ; it != owner.getVMList().end() ; ++it )
    if( vm->getLoadNbTasks() > (*it)->getLoadNbTasks() )
      vm = *it;

  return vm;
}

VM* VMSelection::rate( User& owner, Task& task ) {
  vector<VM*>& vmL = owner.getVMList();
  auto it = owner.getVMList().begin();
  VM*  vm = *it;
  for( ; it != owner.getVMList().end() ; ++it )
    if( vm->getVMips() > (*it)->getVMips() )
      vm = *it;

  return vm;
}

VM* VMSelection::selectVMByNbInstructions( User& owner, Task& task ) {
  vector<VM*>& vmL = owner.getVMList();
  auto it = owner.getVMList().begin();
  VM*  vm = *it;
  for( ; it != owner.getVMList().end() ; ++it )
    if( vm->getLoadNbInstructions() > (*it)->getLoadNbInstructions() )
      vm = *it;

  return vm;
}

Host* HostSelection::fixed( Node& node, Instance& vm ) {
  return *(node.getHostsList().begin());
}

Host* HostSelection::random( Node& node, Instance& vm ) {
  vector<Host*>& hostsL = node.getHostsList();
  return hostsL[rand()%hostsL.size()];
}

Host* HostSelection::circular( Node& node, Instance& vm ) {
  static int p = 0;
  Host *choice = node.getHostsList()[p];

  ++p;
  p = (p == node.getHostsList().size()) ? 0 : p;
  
  return choice;
}

Host* HostSelection::rate( Node& node, Instance& vm ) {
  vector<Host*>& hostsL = node.getHostsList();
  Host  *choice = *hostsL.begin();
  float  cRate   = choice->getRate();

  cout << "Rate [" << choice->getId() << "] = " << choice->getRate() << endl;

  for( auto it = hostsL.begin() + 1 ; it != hostsL.end() ; ++it ) {
    cout << "Rate [" << (*it)->getId() << "] = " << (*it)->getRate() << endl;
    if( (*it)->getRate() > cRate ) {
      choice = *it;
      cRate   = choice->getRate();
    }
  }
  
  return choice;
}

Host* HostSelection::bestFit( Node& node, Instance& vm ) { return (Host*) NULL; }
Host* HostSelection::worstFit( Node& node, Instance& vm ) { return (Host*) NULL; }

Host* VMMigrationHostSelection::randomNodeReceiver( Host& source ) { 
  if( source.getNode()->getHostsList().size() == 1 ) return (Host*)NULL;
  int n = rand()%(source.getNode()->getHostsList().size()-1);
  Host *choice = source.getNode()->getHostsList()[n];
  if( *choice == source ) choice = source.getNode()->getHostsList()[n+1];
  if( source.getNode()->getId() != choice->getNode()->getId() ) abort();
  return (Host*) choice;
}

Host* VMMigrationHostSelection::randomCloudReceiver( Host& source ) { 
  Host *choice;
  do {
    int n = rand()%(Host::getHostsCount()-1);
    if( source.getId() == n ) ++n;
    choice = Host::getHostPtrById(n);
    cout << n << endl;
  } while( choice->isPublic() == true );
  return (Host*) choice;
} 

Host* VMMigrationHostSelection::circularNodeReceiver( Host& source ) {
  return (Host*) NULL;
}

Host* VMMigrationHostSelection::circularCloudReceiver( Host& source ) { 
  static int n = 0;
  if( n == source.getId() ) ++n;
  if( n >= Host::getNbHosts() ) n = 0;
  return (Host*) Host::getHostPtrById(n++);
}

VM* VMMigrationVMSelection::random( Host& sender ) {
  if( sender.getVMMap().size() <= 1 ) return (VM*)NULL;
  auto it = sender.getVMMap().begin();
  std::advance(it,rand()%sender.getVMMap().size());
  return (VM*) it->second;
}

