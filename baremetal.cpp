#include "baremetal.h"
#include "virtualmachine.h"

void BareMetal::place( Instance *vm ) { occupedRam += vm->getVRam(); }
void BareMetal::unplace( Instance *vm ) { occupedRam -= vm->getVRam(); }
bool BareMetal::fitRam( Instance *vm ) { return (ram-occupedRam) >= vm->getVRam(); }

ostream& operator<<( ostream& out, BareMetal& n ) {
  out << "[" << n.id << "|" << n.hostName << "]";
  return out;
}

