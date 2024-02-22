#ifndef __cloud_h__
#define __cloud_h__

#include <vector>
#include <map>

#include "host.h"
#include "baremetal.h"
#include "user.h"
#include "component.h"

class Cloud : public Component {
  static vector<vector<int>> link;
  static map<string,vector<BareMetal*>*> nodesM;
  static vector<BareMetal*> publicHostsL;
public:
  // n: number of hosts
  // c: number of cores per host
  static Host* newHost( const string name, const int risingDate, const int bmFamilly );

  static void readCloudFile( string cloudFileName = "input/mycloud.cld");
  static void readNetworkFile( string networkFileName = "input/network.net");
  static void setLinkSpeeds( int host, string& speeds );
  static inline int getLinkSpeed( int source, int destination ) { return bandwidth(source,destination); }
  static int getLinkSpeed( int source, vector<int>& vSpeeds );
  static void deploy();
  static int  bandwidth(int source, int destination);
  static Host* getHostPtrById( int hostId );

  static inline int uncompletedTasks() { return Task::getNbTasks() - Task::getCompletedTasks(); }

  static void printAllLinks();
  static void printAllCloud();
  static void pushHost(string name, BareMetal *host);
  static void pushPublicHost(string name, BareMetal *host);
  static Node *getNode(string name);
  static int delay( Instance *vm, Host *src, Host *dst );
  static map<string,vector<BareMetal*>*>& getNodesM() { return nodesM; }
  static vector<BareMetal*>& getPublicHostsL() { return publicHostsL; }
};

#endif
 
