#include <iostream>
#include <string>
#include <string.h>

#include "event.h"
#include "bot.h"
#include "user.h"
#include "cloud.h"
#include "host.h"
#include "scheduler.h"
#include "simulator.h"

int Event::eventCount = 0;

// ------- ALL EVENTS
Event::Event( int date, int prior )
   : date(date), prior(prior) {
  eventId = eventCount++;
  Simulator::newEvent(this);
}

bool operator<( const Event &e1, const Event &e2 ) { return e1.date < e2.date; }
bool operator<=( const Event &e1, const Event &e2 ) { return e1.date <= e2.date; }

ostream& operator<<( ostream& out, Event &e ) {
  out << "[" << e.nameEv << "]" << e.eventName()
      << "(" << e.eventId << "): " << e.getDate();
  return out;
}

// -------- BOT EVENTS 
BoTReadyEv::BoTReadyEv( BoT *bot ) : Event(GlobalClock::get(), 0), bot(bot) {
  nameEv = strdup(__func__);
  nameComp = bot->getComponentName();
  idComp   = bot->getId();
  bot->pushEvent(this);
}

BoTReadyEv::~BoTReadyEv() {
  bot->popEvent();
}

void BoTReadyEv::execute() {
  bot->launch();
}

BoTFinishEv::BoTFinishEv( BoT *bot ) : Event(GlobalClock::get(), 0), bot(bot) {
  nameEv = strdup(__func__);
  nameComp = bot->getComponentName();
  idComp   = bot->getId();
  bot->pushEvent(this);
}

void BoTFinishEv::execute() {
  vector<BoT*> succ = bot->getSuccessorsL();
  for( auto it = succ.begin() ; it != succ.end() ; ++it )
    (*it)->dependenceSatisfied();
  cout << "BoTFT " << bot->getId() << " " << date << endl;
  if( Simulator::lastBoTFinishDate > date ) {
   cout << "Last: " << Simulator::lastBoTFinishDate << " New: " << date << endl;
   abort();
  }
  //cout << "BoT (" << bot->getId() << ") Finish\n";
  Simulator::lastBoTFinishDate = date;
  bot->getOwnerPtr()->setLastBoTFinishDate(date);
  BoT::aBoTFinish();
}

string BoTFinishEv::eventName() {
  return to_string(GlobalClock::get())+string(": BoT(")+to_string(bot->getId())+") completed.";
}

BoTFinishEv::~BoTFinishEv() {
  bot->popEvent();
}

// -------- USER EVENTS 
UserLoginEv::UserLoginEv( User* user )
    : Event(user->getUserLoginDate(),2), user(user) {
  nameEv = strdup(__func__);
  nameComp = user->getComponentName();
  idComp   = user->getId();
  user->pushEvent(this);
}

void UserLoginEv::execute() {
  user->userLogin();
}

UserLoginEv::~UserLoginEv() {
  user->popEvent();
}

// --------- NODE EVENTS
HostRisingEv::HostRisingEv( Host* host ) : Event(host->getRisingDate(),0), host(host) {
  nameEv = strdup(__func__);
  nameComp = host->getComponentName();
  idComp   = host->getId();
  host->pushEvent(this);
}

HostRisingEv::~HostRisingEv() {
  host->popEvent();
}

void HostRisingEv::execute() {
  host->setStatus(online);
}

string HostRisingEv::eventName() {
  return to_string(date)+string(": Host(")+host->getName()+string(") Rising.");
}

HostShutdownEv::HostShutdownEv( Host* host ) : Event(GlobalClock::get(),0), host(host) {
  nameEv = strdup(__func__);
  nameComp = host->getComponentName();
  idComp   = host->getId();
  host->pushEvent(this);
}

HostShutdownEv::~HostShutdownEv() {
  host->popEvent();
}

void HostShutdownEv::execute() {
  host->setStatus(offline);
}

string HostShutdownEv::eventName() {
  return to_string(date)+string(": Host")+host->getName()+string(") Shutdown.");
}

// --------- TASK EVENTS
TaskFinishEv::TaskFinishEv( Task *task, int date ) : Event(date,0), task(task) {
  nameEv = strdup(__func__);
  nameComp = task->getComponentName();
  idComp   = task->getId();
  task->pushEvent(this);
}

TaskFinishEv::~TaskFinishEv() { 
  task->popEvent();
}

void TaskFinishEv::execute() {
  task->getVMRunning()->popTask(task);
  Simulator::lastTaskFinishDate = date;
  //cout << "Task (" << task->getId() << ") Finish\n";
}

InstanceSuspendEv::InstanceSuspendEv(Instance *vm, int date) : Event(date,0), vm(vm) {
  nameEv = strdup(__func__);
  nameComp = vm->getComponentName();
  idComp   = vm->getId();
  vm->pushEvent(this);
}

InstanceSuspendEv::~InstanceSuspendEv() {
  vm->popEvent();
}

void InstanceSuspendEv::execute() {
  vm->suspend();
}

InstanceResumeEv::InstanceResumeEv(Instance *vm, int date) : Event(date,0), vm(vm) {
  nameEv = strdup(__func__);
  nameComp = vm->getComponentName();
  idComp   = vm->getId();
  vm->pushEvent(this);
}

InstanceResumeEv::~InstanceResumeEv() {
  vm->popEvent();
}

void InstanceResumeEv::execute() {
  vm->resume();
}

SenderInitiatedMigrationEv::SenderInitiatedMigrationEv(Host *host, int date)
       	: Event(date,0), sender(host) {
  nameEv = strdup(__func__);
  nameComp = sender->getComponentName();
  idComp   = sender->getId();
  sender->pushEvent(this);
}

SenderInitiatedMigrationEv::~SenderInitiatedMigrationEv() {
  sender->popEvent();
}

void SenderInitiatedMigrationEv::execute() {
  Host *receiver; 
  Instance* vm;

  if( sender->getVMMap().size() == 0 ) return;
  vm = sender->getVMMap()[0];
  receiver = Scheduler::receiverNodeSelection(*(vm->getRunningHost()));
  vm->migrate(receiver);
  //new InstanceResumeEv(vm,GlobalClock::get()+Cloud::delay(vm,sender,receiver));
  new InstanceResumeEv(vm,GlobalClock::get()+1);
}
