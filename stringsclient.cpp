///////////////////////////////////////////
//
//    stringsclient.cpp    
//   
///////////////////////////////////////////

#include <string>
using namespace std;
#include "strings.idl"   // include the idl file

#include "rpcproxyhelper.h"
#include "c150debug.h"
#include "c150grading.h"
#include <fstream>

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
     
    // grademe
    GRADEME(argc, argv);
    
    // call functions
    try {
      string result; 
      // set up the socket so proxies can find it
      rpcproxyinitialize(argv[serverArg]);

       printf("Calling upcase(test)\n");
       result = upcase("test");
       printf("Returned from upase(test). Result=%s\n",result.c_str());

       printf("Calling concat(hello,world)\n");
       result = concat("hello","world");
       printf("Returned from concat(hello,world). Result=%s\n",result.c_str());

       printf("Calling concat3(hello, 123 ,world)\n");
       result = concat3("hello"," 123 ","world");
       printf("Returned from concat3(hello, 123 ,world. Result=%s\n",result.c_str());

     } catch (C150Exception e) {
        // write to debug log
        c150debug->printf(C150ALWAYSLOG,"Caught C150Exception: %s\n", e.formattedExplanation().c_str());
        // print
        cerr << argv[0] << ": caught C150NetworkException: " << e.formattedExplanation() << endl;
     }
     return 0;
}

