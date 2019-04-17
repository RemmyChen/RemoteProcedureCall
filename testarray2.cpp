#include <string>
using namespace std;
#include "testarray2.idl"   // include the idl file

#include <iostream>
#include <cstdio>


int sqrt(int x[24], int y[24][15], int z[24][15]) {
  printf("sqrt invoked\n");
  return y[7][9];
}

