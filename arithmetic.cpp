////////////////////////////////////////////////////////
//
//    arithmetic.cpp
//    author: Noah Mendelsohn         
// 
//    Trivial implementations of the routines declared
//    in arithmetic.idl. These are for testing: they
//    just print messages.
//     
////////////////////////////////////////////////////////

#include "arithmetic.idl" // include the idl file 

#include <cstdio>

int add(int x, int y) {
  printf("server add invoked with %d %d Result: %d\n", x, y, x+y);
  return x+y;
}

int subtract(int x, int y) {
  printf("server subtract invoked with %d %d Result: %d\n", x, y, x-y);
  return x-y;
}

int multiply(int x, int y) {
  printf("server multiply invoked with %d %d Result: %d\n", x, y, x*y);
  return x*y;
}

int divide(int x, int y) {
  printf("server divide invoked with %d %d Result: %d\n", x, y, x/y);
  return x/y;
}

