///////////////////////////////////////////
//
//    arithmeticclient.cpp
//    author: Jialu Wei, Remmy Chen      
//   
///////////////////////////////////////////


#include "arithmetic.idl"   // include the idl file
#include "rpcproxyhelper.h"
#include "c150debug.h"
#include "c150grading.h"
#include <fstream>

using namespace std;          // for C++ std library
using namespace C150NETWORK;  // for all the comp150 utilities 

// forward declarations
void setUpDebugLogging(const char *logname, int argc, char *argv[]);

// command line argument
const int serverArg = 1;

// main
int main(int argc, char *argv[]) {
    // set up debug
    setUpDebugLogging("simplefunctionclientdebug.txt",argc, argv);

    // check command line args
    if (argc != 2) {
      fprintf(stderr,"Correct syntxt is: %s <servername> \n", argv[0]);
      exit(1);
    }
     
    // grademe
    GRADEME(argc, argv);
    
    // call functions
    try {
      int result; 
      // set up the socket so proxies can find it
      rpcproxyinitialize(argv[serverArg]);

       printf("Calling add(10,2)\n");
       result = add(10,2);
       printf("Returned from add(10,2). Result=%d\n",result);

       printf("Calling subtract(10,2)\n");
       result = subtract(10,2);
       printf("Returned from subtract(10,2). Result=%d\n",result);

       printf("Calling multiply(10,2)\n");
       result = multiply(10,2);
       printf("Returned from multiply(10,2). Result=%d\n",result);

       printf("Calling divide(10,2)\n");
       result = divide(10,2);
       printf("Returned from divide(10,2). Result=%d\n",result);
     } catch (C150Exception e) {
        // write to debug log
        c150debug->printf(C150ALWAYSLOG,"Caught C150Exception: %s\n", e.formattedExplanation().c_str());
        // print
        cerr << argv[0] << ": caught C150NetworkException: " << e.formattedExplanation() << endl;
     }
     return 0;
}


// set up debug
void setUpDebugLogging(const char *logname, int argc, char *argv[]) {
     ofstream *outstreamp = new ofstream(logname);
     DebugStream *filestreamp = new DebugStream(outstreamp);
     DebugStream::setDefaultLogger(filestreamp);

     //  Put the program name and a timestamp on each line of the debug log.
     c150debug->setPrefix(argv[0]);
     c150debug->enableTimestamp(); 

     // Ask to receive all classes of debug message
     c150debug->enableLogging(C150ALLDEBUG | C150RPCDEBUG | C150APPLICATION | C150NETWORKTRAFFIC | 
			      C150NETWORKDELIVERY); 
}
