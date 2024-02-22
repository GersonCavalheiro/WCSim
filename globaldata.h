#ifndef __globaldata_h__
#define __globaldata_h__

#include <list>
#include <set>

struct GlobalData {
//  static vector<BoT> bot;
  static vector<Host> cloud;
//  vector<User> users;
  static list<Task*> taskList;
};

//vector<BoT> GlobalData::bot;
vector<Host> GlobalData::cloud;
list<Task*> GlobalData::taskList;

#endif
