/* Extract all integers from string */
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <bits/stdc++.h>
#include <sys/time.h>
#include <list>
#include <set>
#include <chrono>

#include "simulator.h"
#include "event.h"
#include "utils.h"
#include "globalclock.h"
#include "virtualmachine.h"
#include "task.h"
#include "bot.h"
#include "user.h"
#include "host.h"
#include "cloud.h"
#include "scheduler.h"
#include "usage.h"
#include "filenames.h"

using namespace std;

static void startSequence() {
  struct timeval t;
  srand(gettimeofday(&t,&t));
  Cloud::deploy();
  //Cloud::printAllCloud();
  User::onboarding();
  BoT::load();
}

int main( int argc, char **argv) {
  if( argc != 5 ) {
    cout << "Usage:\n\twcsim dobFile cloudFile passwordFile networkFile" << endl;
    abort();
  } else {
    FileNames::setDoBFileName(argv[1]);
    FileNames::setCloudFileName(argv[2]);
    FileNames::setPasswordFileName(argv[3]);
    FileNames::setNetworkFileName(argv[4]);
    cout << "Using:\n\twcsim " << "\tDoB:\t " << FileNames::getDoBFileName() << endl
	                       << "\t\tCloud:\t " << FileNames::getCloudFileName() << endl
	                       << "\t\tPassord: " << FileNames::getPasswordFileName() << endl
	                       << "\t\tNetwork: " << FileNames::getNetworkFileName() << endl;

  }
  cout << "Scheduling: "
#ifdef FIXEDSCH
       << " fixed, "
#endif
#ifdef RANDOMSCH
       << " random"
#endif
#ifdef NODESCH
       << ", intra node"
#endif
#ifdef CLOUDSCH
       << ", in the cloud"
#endif
#ifdef BURSTSCH
       << ", bursting"
#endif
       << endl;
  startSequence();

//  Cloud::printAllCloud();
//  User::printAllUsers();
//  BoT::printAllBoTs();
//  VM::printAllVMs();
//  Cloud::printAllLinks();
  
  auto start = chrono::high_resolution_clock::now();
  Simulator::run();
  auto end = chrono::high_resolution_clock::now();

  cout << "++++++\n";
  cout << "Global::Clock: " << GlobalClock::get() << endl;
  cout << "Tasks - Total: " << Task::getNbTasks() << ", Running: "
       << Task::getRunningTasks() << ", Completed: "
       << Task::getCompletedTasks() << endl;
  cout << "++++++\n";
  Usage::print();
  cout << "++++++\n";
  User::charge();
  cout << "++++++\n";
  cout << "Last BoT execution at:  " << Simulator::lastBoTFinishDate << endl;
  cout << "Last Task execution at: " << Simulator::lastTaskFinishDate << endl;
  chrono::duration<double> elapsed = end - start;
  cout << "Simulation time: " << elapsed.count() << "s" << endl;

  Cloud::printAllCloud();

  return 0;
}
