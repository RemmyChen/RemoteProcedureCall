/////////////////////////////////////////////////////////////////////////////
//
//    rpcproxyhelper.cpp
//    author: Noah Mendelsohn 
//
//    This file provides a helper routine to open a socket and put 
//    it in a global variable where individual proxy routines can find it.    
//   
/////////////////////////////////////////////////////////////////////////////

#include "rpcproxyhelper.h"

using namespace C150NETWORK;  // for all the comp150 utilities 

// global variable where proxies can find socket.
C150StreamSocket *RPCPROXYSOCKET;

 
// opens the socket and leaves it in global variable.
void rpcproxyinitialize(char *servername) {
  c150debug->printf(C150RPCDEBUG,"rpcproxyinitialize: Creating C150StreamSocket");
  RPCPROXYSOCKET = new C150StreamSocket();
  // Tell the Streamsocket which server to talk to (port number per student)
  RPCPROXYSOCKET -> connect(servername);  
}
