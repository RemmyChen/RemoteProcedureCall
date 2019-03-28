/////////////////////////////////////////////////////////////////////////////
//
//    rpcserver.cpp
//    author: Noah Mendelsohn 
//
//    This is a skeleton of a server program for RPC. This program loops 
//    accepting connections on a stream socket. On each connection it loops 
//    calling the main stub entry "dispatchFunction()" to invoke one function
//    for the remote client.
//   
/////////////////////////////////////////////////////////////////////////////


#include "rpcstubhelper.h"
#include "c150debug.h"
#include "c150grading.h"
#include <fstream>
#include <sstream>

using namespace std;          // for C++ std library
using namespace C150NETWORK;  // for all the comp150 utilities 

// forward declaration
void setUpDebugLogging(const char *logname, int argc, char *argv[]);

// main
int main(int argc, char *argv[]) {
  // set up debug
  stringstream logfilename;
  logfilename << argv[0] << "debug.txt"; 
  setUpDebugLogging(logfilename.str().c_str(),argc, argv);
  
  // check command args
  if (argc != 1) {
    fprintf(stderr,"Correct syntxt is: %s ... (there are no arguments) \n", argv[0]);
    exit(1);
  }

  // grademe
  GRADEME(argc, argv);

  // call functions
  try {
    // set up socket so stubs can find it
    rpcstubinitialize();

    // infinite loop accepting connections
    while (1) {
	    c150debug->printf(C150RPCDEBUG,"rpcserver.cpp:" "calling C150StreamSocket::accept");
      RPCSTUBSOCKET -> accept();
	 
      // infinite loop processing messages
      while(1)	{
  	    // call a function for the client. The stubs will do the 
  	    // work of reading and writing the stream, but will return on eof,
  	    // which we'll get when client goes away.
        dispatchFunction();
        if (RPCSTUBSOCKET -> eof()) {
          c150debug->printf(C150RPCDEBUG,"rpcserver.cpp: EOF signaled on input");
          break;
        }
      }

      // Done looping on this connection, close and wait for another
      c150debug->printf(C150RPCDEBUG,"Calling C150StreamSocket::close");
	    RPCSTUBSOCKET -> close();
    }
  } catch (C150Exception e) {
    c150debug->printf(C150ALWAYSLOG,"Caught C150Exception: %s\n", e.formattedExplanation().c_str());
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

