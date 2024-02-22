#ifndef __UTILS_H__
#define __UTILS_H__

#include <iostream>
#include <list>
// uncomment to disable assert()
// #define NDEBUG
#include <cassert>
 
// Use (void) to silence unused warnings.
#define assertm(exp, msg) assert(((void)(msg), exp))

using namespace std;
 
#define USERS 4
#define VMS   4
#define PROCS 4
#define NODES 4

/*
 * Atributos para o BoT
*/
enum ATTRIBS { botId = 0,   // unique BoT identificator
               ownerId,     // owner identifier
               nDependBoT,  // number of dependences
               arrivalBoT,  // arrival time (steps)
               nbTasks,     // amount of tasks in the BoT
               nbInstructions,// computational cost for each task (millions of instructions)
               memTask,     // size of each task (amount of memory / comm cost)
               firstDepend, // first dependence
               nbOfAttrib   // number of valid attributs
};

/*
 * Estados para as Tasks e BoTs
*/
enum STATUS { waiting_t = 0, // BoT: waiting the arrival time
              ready_t,       // Task/BoT: ready for run
              blocked_t,     // BoT: there are dependences not solved
              running_t,     // Task/BoT: launched
              completed_t,   // Task/BoT: completed
              migrating_t    // BoT: migrating between hosts
};

/*
 * Estados para as VMs e Nos
*/
enum VMSTATUS { alive = 0, // VM/Host is ready and running
                suspended, // VM/Host is suspended (not running)
                migrating  // VM is migrating among hosts
};

/*
 * Estados para o usuário
 */

#define offline 0
#define online  !offline

string extractAllIntegers(string strIn);
int atoi(char *str);
char *itoa(int num, char *str, int base);

/*
 * orderInsertPtrList
 *   IN:  lst  - list<T>: a list of elements of type T* ordered by a criteria
 *        elem - T*: the new element to be inserted in lst
 *   OUT: lst updated with elem
 *
 *   Useful to keep the event list ordered by date/priority
 *   bool operator<(cons T&,const T&) must be present
*/

template<typename T>
void orderInsert( std::list<T>& lst, T elem ) {
  for( auto p = lst.begin() ; p != lst.end() ; ++p )
    if( *p == elem ) { cout << "EXISTE!!!!\n"; abort(); }
  if( (lst.size() > 0) && (*elem <= *(*(lst.begin()))) ) {
    lst.push_front(elem);
    return;
  }
  for( auto p = lst.begin() ; p != lst.end() ; ++p )
     if( (*p)->getId() == elem->getId() ) { cout << "EXISTE  !!!!\n"; abort(); }
  // ------
  auto posic = lst.begin();
  for( ; (posic != lst.end()) && (*(*posic) < *elem) ; ++posic );
  lst.insert(posic, elem);
}

#endif
