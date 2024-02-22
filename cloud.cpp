#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "cloud.h"
#include "node.h"
#include "host.h"
#include "baremetal.h"
#include "event.h"
#include "filenames.h"

vector<vector<int>> Cloud::link;
map<string,vector<BareMetal*>*> Cloud::nodesM;
vector<BareMetal*> Cloud::publicHostsL;

Host* Cloud::newHost( const string name, const int risingDate, const int bmFamily ) {
  // O parametro bmFamily permite escolher qual classe Host usar
  Host *host;
  switch(bmFamily) {
   case 0: host = (Host*) new ThinHost(name,risingDate);
	   break;
   case 1: host = (Host*) new MediumHost(name,risingDate);
	   break;
   case 2: host = (Host*) new LargeHost(name,risingDate);
	   break;
   case 100: host = (Host*) new PublicHost(name,risingDate);
             pushPublicHost(name,(BareMetal*)host);
	     break;
   default : host = new Host(name, risingDate);
  }
  pushHost(name,(BareMetal*)host);
  cout << "Criei: " << name << endl;
  return host;
}

Host* Cloud::getHostPtrById( int hostId )  { return Host::getHostPtrById( hostId ); }

void Cloud::pushHost(string name, BareMetal *host) {
  auto it = nodesM.find(name);
  if( it == nodesM.end() ) {
    nodesM.insert(pair<string,vector<BareMetal*>*>(name,new vector<BareMetal*>));
    it = nodesM.find(name);
  }
  it->second->push_back(host);
}

void Cloud::pushPublicHost(string name, BareMetal *host) {
  publicHostsL.push_back(host);
}

Node *Cloud::getNode(string name) {
  return Node::getNode(name);
}

void Cloud::readCloudFile( string cloudFileName ) {
  string name;
  int hostRisingDate, bmFamily;
  std::ifstream infile(FileNames::getCloudFileName());

  infile >> name >> hostRisingDate >> bmFamily;
  while( !infile.eof() ) {
    Cloud::newHost(name, hostRisingDate, bmFamily);
    infile >> name >> hostRisingDate >> bmFamily;
  }
  cout << "Lido hostFile\n";
}

void Cloud::setLinkSpeeds( int host, string& speeds ) {
  stringstream sIn;
  vector<int> auxv;
  string auxs;
  int number;

  sIn << speeds;

  for( int i = host+1 ; i <= Host::getNbHosts() ; ++i ) {
    sIn >> auxs;
    if( !(stringstream(auxs) >> number) ) exit(0);
    auxv.push_back(number);
  }
  link.push_back(auxv);
}

int Cloud::getLinkSpeed( int source, vector<int>& vSpeeds ) {
  vSpeeds = link[source];
  return vSpeeds.size();
}

void Cloud::readNetworkFile( string networkFileName ) {
  int host = 0;
  string strIn, strOut;
  std::ifstream infile(FileNames::getNetworkFileName());

  std::getline(infile,strIn);
  while( !infile.eof() && host < Host::getNbHosts() ) {
    strOut = extractAllIntegers(strIn);
    if( strOut.size() != 0 ) setLinkSpeeds(host++,strOut);
    std::getline(infile,strIn);
  }
}

void Cloud::printAllLinks() {
  for( int i = 0; i < Host::getNbHosts() ; ++i ) {
    for( int j = 0; j < Host::getNbHosts() ; ++j ) 
      cout << Cloud::bandwidth(i,j) << "*";
    cout << endl;
  }
}

//inline void swap(int &a, int &b ){int aux=a;a=b;b=aux;}

int Cloud::bandwidth( int line, int column ) {
  if( line >= Host::getNbHosts() || column >= Host::getNbHosts() ) return 5;
  if( line == column ) return 0;
  if( line < column ) return link[line][column-1-line];
  else {std::swap(line,column); return link[line][column-1-line]; }
}

void Cloud::deploy() {
  readCloudFile();
  readNetworkFile();
  // Ler arquivo de descrição da cloud
  // Criar os nos
  // Determinar os custos de comunicação entre os nos
}

void Cloud::printAllCloud() {
  for( auto it = nodesM.begin() ; it != nodesM.end() ; ++it ) {
    cout << (*it).first << ":\t";
    for( auto jt = (*it).second->begin() ; jt != (*it).second->end() ; ++jt )
      cout << "\t" << *(*jt) << " ";
    cout << endl;
  }
}

int Cloud::delay( Instance *vm, Host *src, Host *dst ) {
  int mbps = (src->getNodeName() == dst->getNodeName()) ? 10000 : 1000;
  return vm->getVRam()*1024/8/mbps;
}
