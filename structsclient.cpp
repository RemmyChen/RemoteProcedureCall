///////////////////////////////////////////
//
//    structsclient.cpp
//    author: Jialu Wei, Remmy Chen      
//   
///////////////////////////////////////////

#include <string>
using namespace std;
#include "structs.idl"

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
      
      Person p1 = {"Crystal","Blue", 3};
      Person p2 = {"Baby", "Blue", 7};
      Person p3 = {"Deep", "Blue", 9};
      ThreePeople tp;
      tp.p1 = p1;
      tp.p2 = p2;
      tp.p3 = p3;

      rectangle r;
      r.x = 3;
      r.y = 4;

      // call 
      cout << "sending people" << endl;
      Person p = findPerson(tp);
      cout << "result is " << p.firstname << endl;

      // call 
      cout << "sending rectangle" << endl;
      int val = area(r);
      cout << "result is " << val << endl;


     } catch (C150Exception e) {
        // exception handle
        printf("test1client: Caught C150Exception: %s\n", e.formattedExplanation().c_str());
     }
     return 0;
}


