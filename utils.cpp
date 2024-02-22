#include <string>
#include <sstream>
#include "utils.h"

using namespace std;
 
string extractAllIntegers(string strIn) {
  stringstream sIn;
  string strOut, aux;
  int number;

  // Skips comments
  if( strIn[0] != '#' ) {
    sIn << strIn;
    while( !sIn.eof() ) {
      sIn >> aux;
      if( stringstream(aux) >> number )
        strOut += " " + to_string(number);
    }
  }

  return strOut;
}

int atoi(char* str) {
  int res = 0;
 
  for(int i = 0; str[i] ; ++i)
    res = res * 10 + str[i] - '0';
 
  return res;
}

static void reverse(char str[], int length) {
  int start = 0;
  int end = length -1;
  while(start < end) {
    swap(*(str+start), *(str+end));
    start++;
    end--;
  }
}
 
char* itoa(int num, char* str, int base) {
    int  i = 0;
    bool isNegative = false;
 
    if(num == 0) {
      str[i++] = '0';
      str[i] = '\0';
      return str;
    }
 
    if(num < 0 && base == 10) {
      isNegative = true;
      num = -num;
    }
 
    while(num) {
      int rem = num % base;
      str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0';
      num = num/base;
    }
 
    if (isNegative) str[i++] = '-';
 
    str[i] = '\0';
 
    reverse(str, i);

    return str;
}
