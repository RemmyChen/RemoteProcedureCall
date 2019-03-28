///////////////////////////////////////////
//
//    floatarithmetic.proxy.cpp
//    author: Jialu Wei, Remmy Chen      
//   
///////////////////////////////////////////

#include "floatarithmetic.idl"  // include idl file
#include "rpcproxyhelper.h"
#include <cstdio>
#include <cstring>
#include "c150debug.h"

#include <iostream>   // std::cout
#include <string>     // std::string, std::to_string


using namespace C150NETWORK;  // for all the comp150 utilities 
using namespace std;

#define BUFSIZE 1024

float deserialize_float(string val) {
    float result = stof(val.c_str());
    return result;
}

string serialize_int(float val) {
    return to_string(val);    
}

string receive_full_res() {
  unsigned int i;
  char buffer[BUFSIZE];
  char *bufp;    // next char to read
  bool readMsg;  // whether we've read to the end of a message (message is a functional call)
  ssize_t readlen; // amount of data read from socket

  // Read a message from the stream, -1 in size below is to leave room for null
  readMsg = false;
  bufp = buffer;
  for (i=0; i< BUFSIZE; i++) {
    readlen = RPCPROXYSOCKET-> read(bufp, 1);  // read a byte
    // check for eof or error
    if (readlen == 0) {
      break;
    }
    // check for null and bump buffer pointer
    if (*bufp++ == '#') {     // break into messages (function calls)
        readMsg = true;
        break;
    }
  }
  
  // With TCP streams, we should never get a 0 length read
  // except with timeouts or EOF
  if (readlen == 0) {
    c150debug->printf(C150RPCDEBUG,"floatarithmetic.stub: read zero length message, checking EOF");
    if (RPCPROXYSOCKET-> eof()) {
      c150debug->printf(C150RPCDEBUG,"floatarithmetic.stub: EOF signaled on input");
    } else {
      throw C150Exception("floatarithmetic.stub:  unexpected zero length read without eof");
    }
  } else if(!readMsg) {
    // If we didn't get a null, input message was poorly formatted
    throw C150Exception("simplefunction.stub: method name not null terminated or too long");
  }

  // Note that eof may be set here for our caller to check (???)

  string msg(buffer);
  return msg;
}

float add(float x, float y) {
  // msg buffer
  string buf = "";
  string fn = "add";
  string actual_args = to_string(x) + "," + to_string(y);

  buf = fn + "#" + actual_args + "#";

  // send function signature and params
  RPCPROXYSOCKET->write(buf.c_str(), strlen(buf.c_str()));

  string msg = receive_full_res();
  return deserialize_float(msg);
}

float subtract(float x, float y) {
  // msg buffer
  string buf = "";
  string fn = "subtract";
  string actual_args = to_string(x) + "," + to_string(y);

  buf = fn + "#" + actual_args + "#";

  // send function signature and params
  RPCPROXYSOCKET->write(buf.c_str(), strlen(buf.c_str())); // write function name including null

  string msg = receive_full_res();
  return deserialize_float(msg);
}


float multiply(float x, float y) {
  // msg buffer
  string buf = "";
  string fn = "multiply";
  string actual_args = to_string(x) + "," + to_string(y);

  buf = fn + "#" + actual_args + "#";

  // send function signature and params
  RPCPROXYSOCKET->write(buf.c_str(), strlen(buf.c_str())); // write function name including null

  string msg = receive_full_res();
  return deserialize_float(msg);
}

float divide(float x, float y) {
  // msg buffer
  string buf = "";
  string fn = "divide";
  string actual_args = to_string(x) + "," + to_string(y);

  buf = fn + "#" + actual_args + "#";

  // send function signature and params
  RPCPROXYSOCKET->write(buf.c_str(), strlen(buf.c_str())); // write function name including null

  string msg = receive_full_res();
  return deserialize_float(msg);
}

