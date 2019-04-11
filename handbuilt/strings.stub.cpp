///////////////////////////////////////////
//
//    strings.stub.cpp   
//   
///////////////////////////////////////////

#include <string>
using namespace std;
#include "strings.idl"   // include the idl file


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
  int n = RPCSTUBSOCKET->read(buf, INT_LEN);

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


string string_handler() {
  int str_len = int_handler();

  if (str_len > 0) {
    char buf[str_len];
    int n = RPCSTUBSOCKET->read(buf, str_len);

    if (n == 0) {
      // eof
      return "";
    } else if (n < 0) {
      // error
      perror("read error");
      exit(1);
    } else {
      // n > 0
      string res(buf, str_len);
      return res;
    }
  } else {
    // eof on the previous int read
    return "";
  }
}

///////////////////////////////
//    call functions (local)
///////////////////////////////

void __upcase(string s) {
  string res = upcase(s);

  struct Buffer_info b;
  b.buf = (char*) malloc(1);
  b.buf_len = 0;
  string_pack(&b, res);
  RPCSTUBSOCKET->write(b.buf, b.buf_len);
}


void __concat(string s, string t) {
  string res = concat(s, t);

  struct Buffer_info b;
  b.buf = (char*) malloc(1);
  b.buf_len = 0;
  string_pack(&b, res);
  RPCSTUBSOCKET->write(b.buf, b.buf_len);
}

void __concat3(string s, string t, string u) {
  string res = concat3(s, t, u);

  struct Buffer_info b;
  b.buf = (char*) malloc(1);
  b.buf_len = 0;
  string_pack(&b, res);
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
    // check if eof was reached
    // NOTE: The EOF flag is only set once a read tries to read past the end of the file.
    if (func_name.compare("")==0) {
      return;
    }

    if (func_name.compare("upcase")==0) {
      string s = string_handler();
      __upcase(s);
    } else if (func_name.compare("concat")==0) {
      string s = string_handler();
      string t = string_handler();
      __concat(s, t);
    } else if (func_name.compare("concat3")==0) {
      string s = string_handler();
      string t = string_handler();
      string u = string_handler();
      __concat3(s, t, u);
    } else {
      printf("BAD function\n");
    }
  }
}

 



