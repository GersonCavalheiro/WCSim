#ifndef __task_h__
#define __task_h__

#include <stdlib.h>
#include <vector>

#include "utils.h"
#include "bot.h"
#include "virtualmachine.h"
#include "globalclock.h"
#include "component.h"

using namespace std;

class Node;

class Task : public Component {
private:
  static vector<Task*> tasksL;
  static int taskCount, tasksRunning, tasksCompleted;
  vector<int> botAttr;
  STATUS st;
  int taskId, taskOwner, taskBoT;
  Node *taskNode;
  BoT* myBoT;
  VM* vmRunning;
  int lastDataStamp, estimatedFinishDate;
  int miRemaining; // Amount of computation remaining

public:
  Task( BoT* myBoT, vector<int>& botAttr );

  void setStatus( int st );
  inline int getStatus() const { return st; }
  inline void  setVMRunning( VM *vm ) { vmRunning = vm; }
  inline Host* getHostRunning() const { return vmRunning->getRunningHost(); }
  inline int   getHostRunningId() const { return vmRunning->getRunningHostId(); }
  inline VM*   getVMRunning() const { return vmRunning; }
  inline int   getId() const { return taskId; }
  inline int   getNbInstructions() const { return botAttr[nbInstructions]; }
  inline int   getRam() const { return botAttr[memTask]; }
  inline int   getOwner() const { return taskOwner; }
  inline BoT*  getBoT() const { return myBoT; }
  inline void  setDataStamp() { lastDataStamp = GlobalClock::get(); }
  inline int   getDataStamp() const { return lastDataStamp; }
  inline int   getRemainingInstructions() const { return miRemaining; }
  inline void  hup( int mi ) { if( mi == 0 ) abort(); miRemaining -= mi; }
  inline void  suspend() { cout << "Not implemented. Line: " << __LINE__ << endl; }
  inline void  resume() { cout << "Not implemented. Line: " << __LINE__ << endl; }

  static inline int getNbTasks() { return taskCount; }
  static inline int getRunningTasks() { return tasksRunning; }
  static inline int getCompletedTasks() { return tasksCompleted; }
  static inline vector<Task*>& getTasksL() { return tasksL; }
  static void removeFromTaskList( Task *t );
  friend ostream& operator<<( ostream& out, const Task& t );
};

#endif
