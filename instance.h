#ifndef __INSTANCE_H__
#define __INSTANCE_H__
#include <iostream> 
#include <vector>

#include "component.h"

class Host;
class Node;
class Task;
class User;

using namespace std;

class Instance : public Component {
private:
  static vector<Instance*> instanceL;
  static int nbInstances;
  int id;
  int vCores, vMips, vRam; // vMips, vRam per core
  int occupedVRam;
  int nbTasks;
  int status;
  User *owner;
  Node *sourceNode;
  Host *running; // launched, current location

public:
  Instance( Node *n, User *u, int vCores = 4, int vMips = 100, int vRam = 16 );
  inline int getId() { return id; }
  inline void setSourceNode( Node *n )
                 { sourceNode = n; }
  inline Node *getSourceNode() const
                 { return sourceNode; }
  inline void setRunningHost( Host *n )
                 { running = n; }
  inline Host *getRunningHost() const
                 { return running; }
  inline void setStatus( int st )
	         { status = st; }
  inline int  getStatus()
	         { return status; }
  inline User *getOwner()
                 { return owner; }
  int  getRunningHostId();
  virtual inline int  getVCores() const
                         { return vCores; }
  virtual inline int  getVMips() const
                         { return vMips; }
  virtual inline int  getVRam() const
                         { return vRam; }
  virtual inline string getName() const
                         { return string("DefaultVM"); }


  virtual void place( Task *t );
  virtual void unplace( Task *t );
  virtual bool fitRam( Task *t );
  virtual void avanceTask( Task *t, int nbTasks );
  virtual void suspend() = 0;
  virtual void resume() = 0;
  virtual void migrate( int hostId ) = 0; 
  virtual void migrate( Host *host ) = 0; 
  virtual void localSchedule() = 0;
  virtual int  getLoadNbTasks() = 0;
  void goHome();
  static vector<Instance*>& getInstancesL() { return instanceL; }
};

class ThinInstance : public Instance {
public:
  ThinInstance( Node *n, User *u, int vCores = 1, int vMips = 100, int vRam = 4 ) 
     : Instance( n, u, vCores, vMips, vRam ) {}
  inline string getName() const
                         { return string("ThinVM"); }
};

class FatInstance : public Instance {
public:
  FatInstance( Node *n, User *u, int vCores = 8, int vMips = 100, int vRam = 16 ) 
     : Instance( n, u, vCores, vMips, vRam ) {}
  inline string getName() const
                         { return string("FatVM"); }
};

#endif
