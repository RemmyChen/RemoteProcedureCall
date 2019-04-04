////////////////////////////////////////////////////////
//
//    strings.cpp
////////////////////////////////////////////////////////

#include <string>
using namespace std;
#include "strings.idl"   // include the idl file

#include <cstdio>

string upcase(string s) {
  printf("upcase invoked\n");
  return s;
}

string concat(string s, string t) {
  printf("concat invoked\n");
  return s+t;
}


string concat3(string s, string t, string u) {
  printf("concat3 invoked\n");
  return s+t+u;
}
