// --------------------------------------------------------------
//
//                        floatarithmetic.cpp
//
//        Author: Noah Mendelsohn         
//   
//
//        Trivial implementations of the routines declared
//        in floatarithmetic.idl. These are for testing: they
//        just print messages.
//
//       Copyright: 2012 Noah Mendelsohn
//     
// --------------------------------------------------------------

// IMPORTANT! WE INCLUDE THE IDL FILE AS IT DEFINES THE INTERFACES
// TO THE FUNCTIONS WE'RE IMPLEMENTING. THIS MAKES SURE THE
// CODE HERE ACTUALLY MATCHES THE REMOTED INTERFACE

#include <string>
using namespace std;
#include "floatarithmetic.idl"

#include <cstdio>
#include <iostream>

float add(float x, float y) {
  printf("add invoked\n");
  cout << "Result: " << x+y << endl;
  return x+y;
}

float subtract(float x, float y) {
  printf("subtract invoked\n");
  cout << "Result: " << x-y << endl;
  return x-y;
}

float multiply(float x, float y) {
  printf("multiply invoked\n");
  cout << "Result: " << x*y << endl;
  return x*y;
}

float divide(float x, float y) {
  printf("divide invoked\n");
  cout << "Result: " << x/y << endl;
  return x/y;
}

