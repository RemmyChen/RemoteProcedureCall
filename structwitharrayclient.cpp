///////////////////////////////////////////
//
//    structwitharrayclient.cpp    
//   
///////////////////////////////////////////

#include <string>
using namespace std;
#include "structwitharray.idl"

#include "rpcproxyhelper.h"
#include "c150debug.h"
#include "c150grading.h"
#include <fstream>
#include <iostream>
#include <cstdio>
using namespace C150NETWORK;  // for all the comp150 utilities 

// command line argument
const int serverArg = 1;

// forward declarations
void verify_s(s vl);
int get_last_sum(s vl);

// main
int main(int argc, char *argv[]) {
    // check command line args
    if (argc != 2) {
      fprintf(stderr,"Correct syntxt is: %s <servername> \n", argv[0]);
      exit(1);
    }
     
    // todo: uncomment this later
    // GRADEME(argc, argv);
    
    try {
      // set up socket
      rpcproxyinitialize(argv[serverArg]);



      // struct s {
      //   int m1[4];
      //   int m2[4][10];
      //   int m3[4][10][100];
      // };
      struct s vl1;
      struct s vl2;

      for (int i=0; i<4; i++) {
        vl1.m1[i] = 3;
        vl2.m1[i] = 1;
      }

      for (int i=0; i<4; i++) {
        for (int j=0; j<10; j++) {
          vl1.m2[i][j] = 2;
          vl2.m2[i][j] = 2;
        }
      }

      for (int i=0; i<4; i++) {
        for (int j=0; j<10; j++) {
          for (int k=0; k<100; k++) {
            vl1.m3[i][j][k] = 1;
            vl2.m3[i][j][k] = 3;
          }
        }
      }

      // call echo_s
      printf("------echoed_s call 1-------\n");
      s echoed_s = echo_s(vl1, vl2);
      verify_s(echoed_s);
      
      printf("------get_last_sum call 1------\n");
      int res = get_last_sum(vl1);
      printf("get_last_sum returned: %d\n", res);

      // call echo_s
      printf("------echoed_s call 2-------\n");
      s echoed_s2 = echo_s(vl2, vl1);
      verify_s(echoed_s2);
      
      printf("------get_last_sum call 2------\n");
      int res2 = get_last_sum(vl2);
      printf("get_last_sum returned: %d\n", res2);


     } catch (C150Exception e) {
        // exception handle
        printf("test1client: Caught C150Exception: %s\n", e.formattedExplanation().c_str());
     }
     return 0;
}

int get_last_sum(s vl) {
  int last_sum = 0;
  for (int i=0; i<4; i++) {
    for (int j=0; j<10; j++) {
      for (int k=0; k<100; k++) {
        last_sum += vl.m3[i][j][k];
      }
    }
  }
  return last_sum;
}

void verify_s(s vl) {
  int m1_sum = 0;
  int m2_sum = 0;
  int m3_sum = 0;

  for (int i=0; i<4; i++) {
    m1_sum += vl.m1[i];
  }

  for (int i=0; i<4; i++) {
    for (int j=0; j<10; j++) {
      m2_sum += vl.m2[i][j];
    }
  }

  for (int i=0; i<4; i++) {
    for (int j=0; j<10; j++) {
      for (int k=0; k<100; k++) {
        m3_sum += vl.m3[i][j][k];
      }
    }
  }

  printf("m1 sum is: %d. m2 sum is: %d. m3 sum is: %d\n", m1_sum, m2_sum, m3_sum);
}

