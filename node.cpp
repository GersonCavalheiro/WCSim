#include "node.h"
#include "host.h"
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

int Node::nodeCount = 0;
vector<Node*> Node::nodesL;
map<string,Node*> Node::nodesM;

Node::Node(string name) {
  this->name = name;
  id = nodeCount++;
  nodesL.push_back(this);
  nodesM.insert(pair<string,Node*>(name,this));
}

void Node::pushHost(string name, Host *host) {
  auto it = nodesM.find(name);
  Node *node;
  
  if( it == nodesM.end() ) node = new Node(name);
  else node = it->second;

  node->pushHost(host);
}

void Node::pushHost(Host *host) {
  hostsL.push_back(host);
}

void Node::popHost(Host *host) {
  cout << "Not implemented. Line: " << __LINE__ << endl;
  abort();
}

vector<Host*>& Node::getHostsList(string name) {
  auto it = nodesM.find(name);
  
  if( it == nodesM.end() ) {
    cout << "Node \"" << name << "\" dont exists. Line: " << __LINE__ << endl;
    abort();
  }
  return it->second->hostsL;
}
  
vector<Host*>& Node::getHostsList() {
  return hostsL;
}
  
Node *Node::getNode(string name) {
  auto it = nodesM.find(name);
  if( it == nodesM.end() ) return NULL;
  return it->second;
}

Host *Node::selectHost() {
  return *hostsL.begin();
}
