#include <string>
using namespace std;
#include "test1.idl"   // include the idl file

#include <iostream>
#include <cstdio>


Info echo(Info x, Info y) {
  printf("echo invoked\n");
  return y;
}

Person echo_person (Person x, Person y) {
  printf("echo_person invoked\n");
  return y;
}
