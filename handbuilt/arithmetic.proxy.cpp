///////////////////////////////////////////
//
//    arithmetic.proxy.cpp 
//   
///////////////////////////////////////////

#include "arithmetic.idl"  // include idl file
#include "rpcproxyhelper.h"
#include <cstdio>
#include <cstring>
#include "c150debug.h"

#include <iostream>   // std::cout
#include <string>     // std::string, std::to_string


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
  int n = RPCPROXYSOCKET->read(buf, INT_LEN);

  if (n == 0) {
    // eof
    return -1;
  } else if (n < 0) {
    // error
    perror("read error");
    exit(1);
  } else {
    // n > 0
    memcpy(&res, buf, INT_LEN);
    return ntohl(res);
  }
}


///////////////////////////////
//    call functions (remote)
///////////////////////////////

int add(int x, int y) {
  // construct msg
  struct Buffer_info b;
  b.buf = (char*) malloc(1);
  b.buf_len = 0;

  string_pack(&b, "add");
  int_pack(&b, x);
  int_pack(&b, y);

  // send function signature and params
  RPCPROXYSOCKET->write(b.buf, b.buf_len);

  int res = int_handler();
  return res;
}

int subtract(int x, int y) {
  // construct msg
  struct Buffer_info b;
  b.buf = (char*) malloc(1);
  b.buf_len = 0;

  string_pack(&b, "subtract");
  int_pack(&b, x);
  int_pack(&b, y);

  // send function signature and params
  RPCPROXYSOCKET->write(b.buf, b.buf_len);

  int res = int_handler();
  return res;
}


int multiply(int x, int y) {
  // construct msg
  struct Buffer_info b;
  b.buf = (char*) malloc(1);
  b.buf_len = 0;

  string_pack(&b, "multiply");
  int_pack(&b, x);
  int_pack(&b, y);

  // send function signature and params
  RPCPROXYSOCKET->write(b.buf, b.buf_len);

  int res = int_handler();
  return res;
}


int divide(int x, int y) {
  // construct msg
  struct Buffer_info b;
  b.buf = (char*) malloc(1);
  b.buf_len = 0;

  string_pack(&b, "divide");
  int_pack(&b, x);
  int_pack(&b, y);

  // send function signature and params
  RPCPROXYSOCKET->write(b.buf, b.buf_len);

  int res = int_handler();
  return res;
}

