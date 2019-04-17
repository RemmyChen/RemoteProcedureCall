///////////////////////////////////////////
//
//    testarray2client.cpp
//    author: Jialu Wei, Remmy Chen      
//   
///////////////////////////////////////////

#include <string>
using namespace std;
#include "testarray2.idl"

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
      int x[24], y[24][15], z[24][15];
      for (int i=0; i<24; i++) {
          x[i] = i;
      }
      for (int i=0;i<24;i++) {
          for (int j=0;j<15;j++) {
              y[i][j] = i;
              z[i][j] = j;
          }
      }

      cout << "expecting " << y[7][9] << endl;


      // call 
      int val = sqrt(x, y, z);
      cout << "received " << val << endl;

     } catch (C150Exception e) {
        // exception handle
        printf("test1client: Caught C150Exception: %s\n", e.formattedExplanation().c_str());
     }
     return 0;
}

