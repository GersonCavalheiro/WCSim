#include <iostream>
#include <string>
#include <string.h>

#include "utils.h"
#include "task.h"
#include "node.h"
#include "globalclock.h"

vector<Task*> Task::tasksL;
int           Task::taskCount = 0;
int           Task::tasksRunning = 0;
int           Task::tasksCompleted = 0;

Task::Task( BoT *myBoT, vector<int>& botAttr ) : botAttr(botAttr), myBoT(myBoT) {
  componentName = strdup(__func__);
  if( botAttr[nDependBoT] == 0 && botAttr[arrivalBoT] <= GlobalClock::get() ) this->setStatus(ready_t);
  if( botAttr[nDependBoT] == 0 && botAttr[arrivalBoT] > GlobalClock::get() )  this->setStatus(waiting_t);
  taskId = taskCount++;
  this->setStatus(waiting_t);
  taskOwner = botAttr[ownerId];
  taskNode = myBoT->getSourceNodePtr();
  taskBoT = botAttr[botId];
  miRemaining = botAttr[nbInstructions];
  lastDataStamp = GlobalClock::get();
  tasksL.push_back(this);
}

void Task::setStatus( int st ) {
  if( this->st == st )
    if( st == running_t ) {
      cout << "Running!!!!!!!!!!!!!!!!!!!!!!!!: " << getId() << endl;
      abort();
    }
    else if( st == completed_t ) {
      cout << "Completed!!!!!!!!!!!!!!!!!!!!!!: " << getId() << endl;
      abort();
    }
  this->st = (STATUS) st;
  if( st == running_t ) { 
    ++tasksRunning;
    myBoT->aTaskRunning();
  }
  if( st == completed_t ) {
    ++tasksCompleted;
    --tasksRunning;
    myBoT->aTaskCompleted();
    Task::removeFromTaskList(this);
  }
}

void Task::removeFromTaskList( Task *t ) {
  for( auto it = tasksL.begin() ; it != tasksL.end() ; ++it )
    if( t == *it ) {
      tasksL.erase(it);
      return;
    }
  cout << "Vou abortar" << endl;
  abort();
}

ostream& operator<<( ostream& out, const Task& t ) {
  out << "(t" << t.taskId << ":" << t.taskBoT << "," << t.taskOwner
      << "," << t.taskNode->getId() << ":" << t.botAttr[arrivalBoT] << ")";
  return out;
}

