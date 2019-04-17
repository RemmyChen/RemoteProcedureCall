
#include "testarray1.idl" // include the idl file 

#include <cstdio>

int sqrt(int x[24], int y[24]) {
  printf("server sqrt invoked with: \n");
  int res = 0;
  for (int i=0; i<24; i++) {
    printf("%d\n", x[i]);
    res += x[i];
  }
  printf("returning %d\n", res);
  return res;
}
