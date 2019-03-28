///////////////////////////////////////////
//
//    arithmetic.stub.cpp
//    author: Jialu Wei, Remmy Chen      
//   
///////////////////////////////////////////

#include "arithmetic.idl"  // include idl file
#include "rpcstubhelper.h"
#include <cstdio>
#include <cstring>
#include "c150debug.h"
#include <vector>
#include <string>

using namespace C150NETWORK;  // for all the comp150 utilities 
using namespace std;

void getFunctionNamefromStream();

#define BUFSIZE 1024

// These functions: call the actual functions, serialize results, and send to client

struct arith_func_info {
  string fn;
  int arg1;
  int arg2;
};

int deserialize_int(string val) {
    return atoi(val.c_str()); // note: val has a "#" at the end, but atoi ignores any non-integers
}

string serialize_int(int val) {
    return to_string(val);    
}

void __add(int x, int y) {
  int res = add(x, y);
  string res_str = serialize_int(res) + "#";
  RPCSTUBSOCKET->write(res_str.c_str(), res_str.length());
}


void __subtract(int x, int y) {
  int res = subtract(x, y);
  string res_str = serialize_int(res) + "#";
  RPCSTUBSOCKET->write(res_str.c_str(), res_str.length());
}


void __multiply(int x, int y) {
  int res = multiply(x, y);
  string res_str = serialize_int(res) + "#";
  RPCSTUBSOCKET->write(res_str.c_str(), res_str.length());
}


void __divide(int x, int y) {
  // TODO: check y for 0
  int res = divide(x, y);
  string res_str = serialize_int(res) + "#";
  RPCSTUBSOCKET->write(res_str.c_str(), res_str.length());
}

// Pseudo-stub for missing functions.
void __badFunction(const char *functionName) {
  char doneBuffer[5] = "BAD";
  RPCSTUBSOCKET->write(doneBuffer, strlen(doneBuffer)+1);
}


// read from client and return when we got a whole msg
string receive_full_msg() {
  unsigned int i;
  char buffer[BUFSIZE];
  char *bufp;    // next char to read
  bool readMsg;  // whether we've read to the end of a message (message is a functional call)
  ssize_t readlen; // amount of data read from socket
  int count;

  // Read a message from the stream, -1 in size below is to leave room for null
  count = 0;
  readMsg = false;
  bufp = buffer;
  for (i=0; i< BUFSIZE; i++) {
    readlen = RPCSTUBSOCKET-> read(bufp, 1);  // read a byte
    // check for eof or error
    if (readlen == 0) {
      break;
    }
    // check for null and bump buffer pointer
    if (*bufp++ == '#') {     // break into messages (function calls)
      count += 1;
      if (count == 2) {
        readMsg = true;
        break;
      }
    }
  }
  
  // With TCP streams, we should never get a 0 length read
  // except with timeouts or EOF
  if (readlen == 0) {
    c150debug->printf(C150RPCDEBUG,"arithmetic.stub: read zero length message, checking EOF");
    if (RPCSTUBSOCKET-> eof()) {
      c150debug->printf(C150RPCDEBUG,"arithmetic.stub: EOF signaled on input");
    } else {
      throw C150Exception("arithmetic.stub:  unexpected zero length read without eof");
    }
  } else if(!readMsg) {
    // If we didn't get a null, input message was poorly formatted
    throw C150Exception("arithmetic.stub: method name not null terminated or too long");
  }

  // Note that eof may be set here for our caller to check (???)
  string msg(buffer);
  return msg;
}


struct arith_func_info deserialize(string msg) {
  struct arith_func_info afi;
  size_t start;
  size_t end;
  string delimiter = "#";

  // store the function name into the struct
  start = 0;
  end = msg.find(delimiter, start);
  afi.fn = msg.substr(start, end-start);

  // store the functiona args also into the struct
  start = end + 1;
  end = msg.find(delimiter, start);
  string actual_args = msg.substr(start, end-start);
  // separate actual_args into a vector
  vector<string> arg_vals;
  string token;
  delimiter = ",";
  start = 0;
  end = 0;
  while (end < actual_args.length()) {
    end = actual_args.find(delimiter, start);
    if (end == std::string::npos) {
      end = actual_args.length();
    }
    token = actual_args.substr(start, end-start);
    arg_vals.push_back(token);
    start = end+1;
  }

  afi.arg1 = deserialize_int(arg_vals[0]);
  afi.arg2 = deserialize_int(arg_vals[1]);
  return afi;
}

// called when we're ready to read a new invocation request from the stream
void dispatchFunction() {
  // receive_full_msg takes the buffer, read in a whole msg, and store that msg into the buffer
  string msg = receive_full_msg();

  if (!RPCSTUBSOCKET-> eof()) {
    // deserialize (takes the msg -> which function to call, actual arguments)
    struct arith_func_info fi = deserialize(msg);
    cout << "fn name " << fi.fn << endl;
    cout << "fn arg vals " << fi.arg1 << " " << fi.arg2 << endl;

    if (strcmp(fi.fn.c_str(),"add") == 0) {
       __add(fi.arg1, fi.arg2);
    } else if (strcmp(fi.fn.c_str(),"subtract") == 0) {
       __subtract(fi.arg1, fi.arg2);
    } else if (strcmp(fi.fn.c_str(),"multiply") == 0) {
       __multiply(fi.arg1, fi.arg2);
    } else if (strcmp(fi.fn.c_str(),"divide") == 0) {
       __divide(fi.arg1, fi.arg2);
    } else {
       __badFunction(fi.fn.c_str());
     }
  }
}

 



