///////////////////////////////////////////
//
//    testarray1client.cpp    
//   
///////////////////////////////////////////

#include <string>
using namespace std;
#include "testarray1.idl"

#include "rpcproxyhelper.h"
#include "c150debug.h"
#include "c150grading.h"
#include <fstream>
#include <iostream>
#include <cstdio>
using namespace C150NETWORK;  // for all the comp150 utilities 

// command line argument
const int serverArg = 1;


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


      int a[24]; 
      int b[24];
      for (int i=0; i<24; i++) {
        a[i] = 1;
        b[i] = 2;
      }

      printf("---sqrt call 1 ---\n");
      int res = sqrt(a, b);
      printf("sqrt returned: %d\n", res);

      printf("---sqrt call 2 ---\n");
      res = sqrt(b, a);
      printf("sqrt returned: %d\n", res);


     } catch (C150Exception e) {
        // exception handle
        printf("testarray1client: Caught C150Exception: %s\n", e.formattedExplanation().c_str());
     }
     return 0;
}
