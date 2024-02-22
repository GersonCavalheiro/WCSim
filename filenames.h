#ifndef __files_h__
#define __files_h__

#include <string>

using namespace std;

struct FileNames {
  static void   setDoBFileName(string name) { dobFileName = "input/"+name+".dob"; }
  static string getDoBFileName() { return dobFileName; }
  static void   setCloudFileName(string name) { cloudFileName = "input/"+name+".cld"; }
  static string getCloudFileName() { return cloudFileName; }
  static void   setPasswordFileName(string name) { passwordFileName = "input/"+name+".pwd"; }
  static string getPasswordFileName() { return passwordFileName; }
  static void   setNetworkFileName(string name) { networkFileName = "input/"+name+".net"; }
  static string getNetworkFileName() { return networkFileName; }

private:
  static string dobFileName, cloudFileName, passwordFileName, networkFileName;
};

#endif
