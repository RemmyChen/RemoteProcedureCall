
#include "structwitharray.idl"   // include the idl file

#include <iostream>
#include <cstdio>


s echo_s(s x, s y) {
  printf("echo_s invoked\n");
  return y;
}
