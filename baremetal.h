#ifndef __baremetal_h__
#define __baremetal_h__

#include <climits>
#include <map>

#include "component.h"
#include "virtualmachine.h"
#include "usage.h"

class BareMetal : public Component {
private:
  int  cores, mips, ram, storage, gpu;
  int  occupedCores, occupedRam, occupedStorage;
  int  lastDataStamp;
  bool pinnedGpu;
protected:
  string nodeName, hostName;
  int    id, risingDate, status, family;
public:
  /* Default:
   *         4 cores            (number of cores)
   *         10 MPIS per core   (speed)
   *         64 GB RAM Total    (4 GB per core)
   *         unlimited storage  (INT_MAX TB)
   *         GPU non avalable   (false)
  */
  BareMetal( int cores = 4, int mips = 100000, int ram = 16, int storage = INT_MAX, bool gpu = false )
       : cores(cores), mips(mips),
         ram(ram*cores), storage(storage), gpu(gpu) {
    lastDataStamp = occupedRam = occupedCores = occupedStorage = 0;
    pinnedGpu = false;
    family = 0;
  }
  inline virtual int   getId() const = 0;
  inline string getHostName() const
                           { return hostName; }
  inline string getNodeName() const
                           { return nodeName; }
  inline virtual int   getCores() const
                           { return cores; }
  inline virtual int   getMips() const
                           { return mips; }
  inline virtual float getRate() const
                           { return getActualMips(); }
  inline virtual int   getRam() const
                           { return ram; }
  inline virtual bool  getGPU() const
                           { return false; }
  inline virtual int   getStorage() const
                           { return storage; }
  inline virtual int   getFamily() const
                           { return family; }
  inline virtual float getUtilizationRate() const 
                           { return (cores >= occupedCores)
				    ? 1.0
				    : (float)cores/occupedCores; }
  inline virtual int   getActualMips() const
                           { return mips*this->getUtilizationRate(); }
  inline virtual int   miDelivered(int t) const
                           { return this->getUtilizationRate()*mips*t; }
  virtual void pinCore(int c = 1) {
                  Usage::update(this->getId(),
				GlobalClock::get()-getDataStamp(),
				(float)occupedCores/cores);
	          occupedCores += c;
		  setDataStamp();
  }

  virtual void releaseCore(int c = 1) {
                  Usage::update(this->getId(),
				GlobalClock::get()-getDataStamp(),
				(float)occupedCores/cores);
	          occupedCores = (occupedCores <= c)?0:occupedCores-c;
		  setDataStamp();
  }

  virtual void place( Instance *vm );
  virtual void unplace( Instance *vm );
  virtual bool fitRam( Instance *vm );
  virtual bool isPublic() const { return false; }
  virtual  inline map<int,Instance*>&   getVMMap() = 0;
  void setDataStamp()
	  { lastDataStamp = GlobalClock::get(); }
  int  getDataStamp() { return lastDataStamp; }
  friend ostream& operator<<( ostream& out, BareMetal& n );
  inline friend bool operator==( BareMetal& a, BareMetal& b ) { return a.id == b.id; }
};

#endif


