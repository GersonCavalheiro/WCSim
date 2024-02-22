#include <vector>
#include <map>
#include "usage.h"
#include "host.h"

//map<int,int*> Usage::usage;
map<int,vector<int>> Usage::usage;

void Usage::print() {
    cout << "------------\n";
    for( auto it = usage.begin() ; it != usage.end() ; ++it ) {
    cout << "Host: " << it->first << "(" << Host::getName(it->first) << ")" << endl;
    cout << "\t" << "Idle:\t\t" << it->second[0] << endl;
    for( int i = 1 ; i < 8 ; ++i )
       cout << "\t" << (int)(i-1)*0.25*100 << "-" << (int)i*0.25*100 << "%:"
	    << ((i<4) ? "\t\t" : "\t")
            << it->second[i] << endl;
    cout << "\t" << ">= 200%:\t" << it->second[8] << endl;
    cout << "------------\n";
  }

  ofstream trace("output/trace.csv", ios::out);
  trace << "Host,HostId,Idle,25%,50%,75%,100%,125%,150%,175%,>= 200%" << endl;
  for( auto it = usage.begin() ; it != usage.end() ; ++it ) {
    trace << Host::getName(it->first) << "," << it->first;
    for( int i = 0 ; i < 9 ; ++i )
      trace << "," << it->second[i];
    trace << endl;
  }
  trace.close();
}
