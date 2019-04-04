///////////////////////////////////////////
//
//    strings.proxy.cpp 
//   
///////////////////////////////////////////
#include <string>
using namespace std;
#include "strings.idl"   // include the idl file

#include "rpcproxyhelper.h"
#include <cstdio>
#include <cstring>
#include "c150debug.h"

#include <iostream>   // std::cout


using namespace C150NETWORK;  // for all the comp150 utilities 


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

///////////////////////////////
//    call functions (remote)
///////////////////////////////
string upcase(string s) {
  // construct msg
  struct Buffer_info b;
  b.buf = (char*) malloc(10);
  b.buf_len = 0;

  string_pack(&b, "upcase");
  string_pack(&b, s);

  // send function signature and params
  RPCPROXYSOCKET->write(b.buf, b.buf_len);
  free(b.buf); //
  string res = string_handler();
  return res;
}


string concat(string s, string t) {
    // construct msg
  struct Buffer_info b;
  b.buf = (char*) malloc(10);
  b.buf_len = 0;

  string_pack(&b, "concat");
  string_pack(&b, s);
  string_pack(&b, t);

  // send function signature and params
  RPCPROXYSOCKET->write(b.buf, b.buf_len);
  free(b.buf); //

  string res = string_handler();
  return res;
}

string concat3(string s, string t, string u) {
  // construct msg
  struct Buffer_info b;
  b.buf = (char*) malloc(10);
  b.buf_len = 0;

  string_pack(&b, "concat3");
  string_pack(&b, s);
  string_pack(&b, t);
  string_pack(&b, u);

  // send function signature and params
  RPCPROXYSOCKET->write(b.buf, b.buf_len);
  free(b.buf); //

  string res = string_handler();
  return res;
}
