#ifndef __host_h__
#define __host_h__

#include <iostream>
#include <list>
#include <map>

#include "utils.h"
#include "baremetal.h"
#include "cloud.h"
#include "task.h"
#include "virtualmachine.h"
#include "component.h"
#include "usage.h"

class Host : public BareMetal {
  static map<int,Host*>    hostsListById;
  static map<string,Host*> hostsListByName;
  static int hostCount;
  map<int,Instance*> vmM;
  list<Task*> taskList; // Lista de tarefas prontas
  int nbCores;
  //int id, risingDate, status;
  //string nodeName, hostName;
  char *className;

public:
  Host( const string name, const int risingDate, int cores = 4, int mips = 100000, int ram = 16, int storage = INT_MAX, bool gpu = false );
  inline int    getId()                const { return id; }
  static inline int    getHostsCount() { return hostCount; }
  static string getName(int id)       { return hostsListById[id]->getName(); }
  static string getNodeName(int id)       { return hostsListById[id]->getNodeName(); }
  inline int    getHostNb(int hostId) const
    { return hostsListById[hostId]->getId(); }
  inline int    getRisingDate()       const { return risingDate; }
  inline int    getStatus()           const { return status; }
  inline bool   isOnline()            const { return status == online; }
  inline void   setStatus( int st )         { status = st; }
  inline string getName() const             { return hostName; }
  inline string getNodeName() const { return nodeName; }
  Node  *getNode();
  inline int    getNbVMs()            const { return vmM.size(); }
//  inline vector<Instance*>&   getVMList()         { return vmL; }
  inline map<int,Instance*>&   getVMMap()         { return vmM; }

  void pushVM( Instance* vm );
  void popVM( Instance* vm );

  // void updateSpeed();

  static inline bool isActive( int hostId )
    { return hostsListById.find(hostId) != hostsListById.end(); }
  static inline int  getNbHosts()
    { return hostCount; }
  static Host* getHostPtrById( int hostId );
  static void insertHostInLists( Host *n );

  //friend ostream& operator<<( ostream& out, Host& n );
  static void printAllHosts();
  void printStatus();
};

class ThinHost : public Host {
public:
  ThinHost( const string name, const int risingDate, int cores = 4, int mips = 100000, int ram = 16, int storage = INT_MAX, bool gpu = false ) : Host(name,risingDate,cores,mips,ram,storage,gpu) { family = 0; }
};

class MediumHost : public Host {
public:
  MediumHost( const string name, const int risingDate, int cores = 12, int mips = 100000, int ram = 256, int storage = INT_MAX, bool gpu = false ) : Host(name,risingDate,cores,mips,ram,storage,gpu) { family = 1; }
};

class LargeHost : public Host {
public:
  LargeHost( const string name, const int risingDate, int cores = 24, int mips = 100000, int ram = 512, int storage = INT_MAX, bool gpu = false ) : Host(name,risingDate,cores,mips,ram,storage,gpu) { family = 2; }
};

class PublicHost : public Host {
public:
  PublicHost( const string name, const int risingDate, int cores = 48, int mips = 100000, int ram = 2048, int storage = INT_MAX, bool gpu = false ) : Host(name,risingDate,cores,mips,ram,storage,gpu) { family = 100; }
  inline bool isPublic() const { return true; }
};


#endif

