/////////////////////////////////////////////////////////////////////////////
//
//    rpcstubhelper.cpp
//    author: Noah Mendelsohn 
//
//    This file provides a helper routine to open a socket and put
//    it in a global variable where individual stub routines can find it.
//
/////////////////////////////////////////////////////////////////////////////

#include "rpcstubhelper.h"

using namespace C150NETWORK;  // for all the comp150 utilities 

// global variable where stubs can find socket.
C150StreamSocket *RPCSTUBSOCKET;
 
// opens the socket and leaves it in global variable.
void rpcstubinitialize() {
  // create new socket
  c150debug->printf(C150RPCDEBUG,"rpcstubinitialize: Creating C150StreamSocket");
  RPCSTUBSOCKET = new C150StreamSocket();
  // Tell the OS to start allowing connections
  // The application will pick those up one at a time by doing accept calls
  RPCSTUBSOCKET -> listen();  
}
