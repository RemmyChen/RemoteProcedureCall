///////////////////////////////////////////
//
//    arithmetic.stub.cpp   
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

#define INT_LEN 4
#define FLOAT_LEN 4

struct Buffer_info {
  char *buf;
  int buf_len;
};


///////////////////////////////
//    serialize 
///////////////////////////////

void int_pack(struct Buffer_info *b,  int i1) {
  int converted = htonl(i1);
  int new_buf_len; 
  char *new_buf;

  new_buf_len= b->buf_len + INT_LEN;
  new_buf = (char*)malloc(new_buf_len);
  memcpy(new_buf, b->buf, b->buf_len);
  memcpy(new_buf+b->buf_len, (char *)&converted, INT_LEN);
  free(b->buf);

  b->buf = new_buf;
  b->buf_len = new_buf_len;
}

void string_pack(struct Buffer_info *b, string s) {
  int_pack(b, s.length());

  int new_buf_len; 
  char *new_buf;

  new_buf_len= b->buf_len + s.length();
  new_buf = (char*)malloc(new_buf_len);
  memcpy(new_buf, b->buf, b->buf_len);
  memcpy(new_buf+b->buf_len, s.c_str(), s.length());
  free(b->buf);

  b->buf = new_buf;
  b->buf_len = new_buf_len;
}


///////////////////////////////
//    deserialize 
///////////////////////////////

int int_handler() {
  char buf[INT_LEN];
  int res;
  int readlen = RPCSTUBSOCKET->read(buf, INT_LEN);
  if (readlen == 0) return 0;
  memcpy(&res, buf, INT_LEN);
  return ntohl(res);
}


string string_handler() {
  int str_len = int_handler();

  char buf[str_len];
  int readlen = RPCSTUBSOCKET->read(buf, str_len);
  if (readlen == 0) return "";
  string res(buf, str_len);
  return res;
}


///////////////////////////////
//    call functions (local)
///////////////////////////////

void __add(int x, int y) {
  int res = add(x, y);

  struct Buffer_info b;
  b.buf = (char*) malloc(1);
  b.buf_len = 0;
  int_pack(&b, res);
  RPCSTUBSOCKET->write(b.buf, b.buf_len);
}


void __subtract(int x, int y) {
  int res = subtract(x, y);
  
  struct Buffer_info b;
  b.buf = (char*) malloc(1);
  b.buf_len = 0;
  int_pack(&b, res);
  RPCSTUBSOCKET->write(b.buf, b.buf_len);
}


void __multiply(int x, int y) {
  int res = multiply(x, y);
  
  struct Buffer_info b;
  b.buf = (char*) malloc(1);
  b.buf_len = 0;
  int_pack(&b, res);
  RPCSTUBSOCKET->write(b.buf, b.buf_len);
}


void __divide(int x, int y) {
  // TODO: check y for 0
  int res = divide(x, y);
  
  struct Buffer_info b;
  b.buf = (char*) malloc(1);
  b.buf_len = 0;
  int_pack(&b, res);
  RPCSTUBSOCKET->write(b.buf, b.buf_len);
}


// Pseudo-stub for missing functions.
void __badFunction(const char *functionName) {
  char doneBuffer[5] = "BAD";
  RPCSTUBSOCKET->write(doneBuffer, strlen(doneBuffer)+1);
}


///////////////////////////////
//    dispatch function
///////////////////////////////

// called when we're ready to read a new invocation request from the stream
void dispatchFunction() {
  printf("IN dispatchFunction\n");
  if (!RPCSTUBSOCKET -> eof()) {
    string func_name = string_handler();
    if (func_name.compare("")==0) {
        return;
    }
    if (func_name.compare("add")==0) {
      int x = int_handler();
      int y = int_handler();
      __add(x, y);
    } else if (func_name.compare("subtract")==0) {
      int x = int_handler();
      int y = int_handler();
      __subtract(x, y);
    } else if (func_name.compare("multiply")==0) {
      int x = int_handler();
      int y = int_handler();
      __multiply(x, y);
    } else if (func_name.compare("divide")==0) {
      int x = int_handler();
      int y = int_handler();
      __divide(x, y);
    } else {
      printf("BAD function\n");
    }
  }
}

 



