///////////////////////////////////////////
//
//    floatarithmetic.proxy.cpp 
//   
///////////////////////////////////////////

// NOTE: we are only supporting float precision up to 6 decimal digits

#include "floatarithmetic.idl"  // include idl file
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

void float_pack(struct Buffer_info *b,  float f1) {
  string s = to_string(f1);
  string_pack(b, s);
}


///////////////////////////////
//    deserialize 
///////////////////////////////

int int_handler() {
  char buf[INT_LEN];
  int res;
  RPCPROXYSOCKET->read(buf, INT_LEN);
  memcpy(&res, buf, INT_LEN);
  return ntohl(res);
}

string string_handler() {
  int str_len = int_handler();

  char buf[str_len];
  RPCPROXYSOCKET->read(buf, str_len);
  string res(buf, str_len);
  return res;
}

float float_handler() {
  string s = string_handler();
  return atof(s.c_str());
}

///////////////////////////////
//    call functions (remote)
///////////////////////////////

float add(float x, float y) {
  // construct msg
  struct Buffer_info b;
  b.buf = (char*) malloc(1);
  b.buf_len = 0;

  string_pack(&b, "add");
  float_pack(&b, x);
  float_pack(&b, y);

  // send function signature and params
  RPCPROXYSOCKET->write(b.buf, b.buf_len);

  float res = float_handler();
  return res;
}

float subtract(float x, float y) {
  // construct msg
  struct Buffer_info b;
  b.buf = (char*) malloc(1);
  b.buf_len = 0;

  string_pack(&b, "subtract");
  float_pack(&b, x);
  float_pack(&b, y);

  // send function signature and params
  RPCPROXYSOCKET->write(b.buf, b.buf_len);

  float res = float_handler();
  return res;
}


float multiply(float x, float y) {
  // construct msg
  struct Buffer_info b;
  b.buf = (char*) malloc(1);
  b.buf_len = 0;

  string_pack(&b, "multiply");
  float_pack(&b, x);
  float_pack(&b, y);

  // send function signature and params
  RPCPROXYSOCKET->write(b.buf, b.buf_len);

  float res = float_handler();
  return res;
}


float divide(float x, float y) {
  // construct msg
  struct Buffer_info b;
  b.buf = (char*) malloc(1);
  b.buf_len = 0;

  string_pack(&b, "divide");
  float_pack(&b, x);
  float_pack(&b, y);

  // send function signature and params
  RPCPROXYSOCKET->write(b.buf, b.buf_len);

  float res = float_handler();
  return res;
}

