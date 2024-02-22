#ifndef __bot_h__
#define __bot_h__

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

#include "component.h"
#include "utils.h"
#include "event.h"
#include "task.h"
#include "simulator.h"
#include "node.h"

using namespace std;

class User;

class BoT : public Component {
  static map<int,BoT*> botsM; // <botId,ptr>
  static int botCount, botFinished;
  int botNum, id;  // internal BoT number (sequential), user privided identification
  vector<int>   initialAttribs;
  vector<BoT*>  successors;
  vector<Task*> tasksL;
  User *owner;
  Node *sourceNode;
  int currentDependences,
      initialPriority, // Para quando a prioridade for inserida
      nbRunningTasks,
      nbCompletedTasks;
  STATUS status;
public:
  BoT(vector<int>& attr);
  void launch();
  inline vector<int>& getBoTAttr() { return initialAttribs; }
  inline STATUS getStatus() const { return status; }
  inline int getNum() const { return botNum; }
  inline int getId() const { return id; }
  inline int getArrivalDate() const { return initialAttribs[arrivalBoT]; }
  inline int getPriority() const { return initialPriority; }
  inline int getNbTasks() const { return initialAttribs[nbTasks]; }
  inline int getNbDepend() const { return initialAttribs[nDependBoT]; }
  inline int getNodeId() const { return sourceNode->getId(); }
  inline int getOwnerId() const { return initialAttribs[ownerId]; }
  inline User* getOwnerPtr() const { return owner; }
  inline Node* getSourceNodePtr() const { return sourceNode; }
  inline int getNonConcludedDependences() const { return currentDependences; }
  inline void aTaskRunning() { ++nbRunningTasks; }
  inline vector<BoT*>& getSuccessorsL() { return successors; }
  void dependenceSatisfied();
  void aTaskCompleted();
  inline void setConcludedDependence() {
    if( --currentDependences < 0 ) {
      cout << "Fail concluding dependence to BoT: " << *this << endl;
      exit(0);
    }
  }
  inline int  getDependence( int d ) const { return initialAttribs[firstDepend+d]; }
  inline void setSuccessor( BoT* b ) { successors.push_back(b); }
  inline void setReady() { status = ready_t; }

  static inline void aBoTFinish() { ++botFinished; }
  static inline int  undoneBoTs() { return botCount - botFinished; }
  static void extractBoTAttributes(vector<int>& attr, string& strIn );
  static void buildBoT( string& attribs );
  static void makeBoTDependences();
  static void readDoBFile();
  static void readBoTFile( string botFileName );
  static void load();
  static void printAllBoTs();

  friend ostream& operator<<( ostream& out, const BoT& b );
  friend bool operator<(const BoT &b1, const BoT &b2 );
};

#endif
