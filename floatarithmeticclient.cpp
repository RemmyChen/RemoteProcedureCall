///////////////////////////////////////////
//
//    floatarithmeticclient.cpp  
//   
///////////////////////////////////////////


#include "floatarithmetic.idl"   // include the idl file
#include "rpcproxyhelper.h"
#include "c150debug.h"
#include "c150grading.h"
#include <fstream>

using namespace std;          // for C++ std library
using namespace C150NETWORK;  // for all the comp150 utilities 


// command line argument
const int serverArg = 1;

// main
int main(int argc, char *argv[]) {
    // set up debug
    //setUpDebugLogging("arithmeticclientdebug.txt",argc, argv);

    // check command line args
    if (argc != 2) {
      fprintf(stderr,"Correct syntxt is: %s <servername> \n", argv[0]);
      exit(1);
    }
     
    // grademe
    // GRADEME(argc, argv);
    
    // call functions
    try {
      float result; 
      // set up the socket so proxies can find it
      rpcproxyinitialize(argv[serverArg]);

       result = add(10.12,2.123);
       cout << "float add. Result=" << result << endl;

       result = subtract(10.12,2.123);
       cout << "float subtract. Result=" << result << endl;

       result = multiply(10.12,2.123);
       cout << "float multiply. Result=" << result << endl;

       result = divide(10.12,2.123);
       cout << "float divide. Result=" << result << endl;

     } catch (C150Exception e) {
        // write to debug log
        c150debug->printf(C150ALWAYSLOG,"Caught C150Exception: %s\n", e.formattedExplanation().c_str());
        // print
        cerr << argv[0] << ": caught C150NetworkException: " << e.formattedExplanation() << endl;
     }
     return 0;
}
