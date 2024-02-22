#ifndef __event_h__
#define __event_h__

#include <string>
#include <iostream>

#include "globalclock.h"

using namespace std;

class BoT;
class User;
class Task;
class Instance;
class Host;
class Node;

class Event {
protected:
  static int eventCount;
  int   date, prior, eventId;
  char *nameEv, *nameComp;
  int   idComp;
public:
  Event( int date = 0, int prior = 0 );
  virtual ~Event() { }
  int getDate() const { return date; }
  int priority() const { return prior; }
  virtual void execute() = 0;
  virtual string eventName() = 0;
  inline int getId() { return eventId; }
  friend bool operator<( const Event &e1, const Event &e2 );
  friend bool operator<=( const Event &e1, const Event &e2 );
  friend ostream& operator<<( ostream& out, Event& e );
};

struct CompareEventsPtrs {
  bool operator()( const Event* e1, const Event* e2 ) const {
    if( e1->getDate() != e2->getDate() ) return e1->getDate() > e2->getDate();
	    return e1->priority() > e2->priority(); // 0 is the largest event priority
  }
};

// -------- BOT EVENTS 
class BoTReadyEv : public Event {
  BoT *bot;
  int  hostId;
public:
  BoTReadyEv( BoT *bot );
  ~BoTReadyEv();
  void execute();
  string eventName() { return to_string(date)+string(": BoT ready"); }
};

class BoTFinishEv : public Event {
  BoT* bot;
public:
  BoTFinishEv( BoT* bot );
  ~BoTFinishEv();
  void execute();
  string eventName();
};

// -------- USER EVENTS 
class UserLoginEv : public Event {
  User *user;
public:
  UserLoginEv( User* user );
  ~UserLoginEv();
  void execute();
  string eventName() { return to_string(date)+string(": User Login"); }
};

// -------- NODE EVENTS 
class HostRisingEv : public Event {
  Host *host;
public:
  HostRisingEv( Host* host );
  ~HostRisingEv();
  void execute();
  string eventName();
};

class HostShutdownEv : public Event {
  Host *host;
public:
  HostShutdownEv( Host* host );
  ~HostShutdownEv();
  void execute();
  string eventName();
};

// -------- TASK EVENTS 
class TaskFinishEv : public Event {
  Task *task;
public:
  TaskFinishEv( Task *task, int date );
  ~TaskFinishEv();
  void execute();
  string eventName() { return to_string(date)+string(": Task Finish"); }
};

/* ----
class TaskReadyEv : public Event {
  Task *task;
public:
  TaskReadyEv( Task *task );
  ~TaskReadyEv();
  void execute();
  string eventName() { return to_string(date)+string(": Task Finish"); }
};
 ---- */

// -------- INSTANCE EVENTS 
class InstanceSuspendEv : public Event {
  Instance *vm;
public:
  InstanceSuspendEv( Instance *vm, int date );
  ~InstanceSuspendEv();
  void execute();
  string eventName() { return to_string(date)+string(": Suspend Instance "); }
};

class InstanceResumeEv : public Event {
  Instance *vm;
public:
  InstanceResumeEv( Instance *vm, int date );
  ~InstanceResumeEv();
  void execute();
  string eventName() { return to_string(date)+string(": Resume Instance "); }
};

// -------- SCHEDULE EVENTS
class SenderInitiatedMigrationEv : public Event {
  Host *sender; //Node launching migration
public:
  SenderInitiatedMigrationEv( Host *host, int date );
  ~SenderInitiatedMigrationEv();
  void execute();
  string eventName() { return to_string(date)+string(": Migration Start "); }
};

#endif
