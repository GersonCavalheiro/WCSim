#ifndef __NODE_H__
#define __NODE_H__

#include <vector>
#include <map>
#include <string>

#include "component.h"

class Host;

using namespace std;

class Node : public Component {
  string name;
  int    id;
  static int nodeCount;
  vector<Host*> hostsL;
  static vector<Node*> nodesL;
  static map<string,Node*> nodesM;

  Node(string name);

public:
  static void pushHost(string name, Host *host);
  void pushHost(Host *host);
  static void popHost(Host *host);
  static vector<Host*>& getHostsList(string name);
  vector<Host*>& getHostsList();
  static Node *getNode(string name);
  inline int getId() { return id; }
  inline string getName() { return name; }
  static vector<Node*>& getNodesList() { return nodesL; }

  Host *selectHost();
};

#endif
