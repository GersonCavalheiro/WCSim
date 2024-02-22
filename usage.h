#ifndef __usage_h__
#define __usage_h__

#include <iostream>
#include <vector>
#include <map>

using namespace std;

/* -------------------------
 * Contabiliza o tempo em que o nó foi utilizado em faixas de uso
 * São consideradas 8 faixas de uso: 0%, 25%, 50% ... 175% e 200% ou mais
 * ------------------------ */
class Usage {
static map<int,vector<int>> usage;

public:
  static void insert( int n ) {
    auto p = pair<int,vector<int>>(n,vector<int>(9));
    for( int i = 0 ; i < 10 ; ++i ) p.second[i] = 0;
    usage.insert(p);
  } 
  static void update( int n, int t, float u ) {
    if( t == 0 ) return;
    int p = (u < 2.0) ? (int) (u/0.25) : 8;
    usage.find(n)->second[p] += t;
  }
  static void print();
};

#endif
