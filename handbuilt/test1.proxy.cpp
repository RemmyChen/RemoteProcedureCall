///////////////////////////////////////////
//
//    test1.proxy.cpp  
//   
///////////////////////////////////////////

#include <string>
using namespace std;
#include "test1.idl"   // include the idl file

#include "rpcproxyhelper.h"
#include "c150debug.h"
#include <cstdio>
#include <cstring>
#include <iostream>
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

void float_pack(struct Buffer_info *b,  float f1) {
  string s = to_string(f1);
  string_pack(b, s);
}

void string_5_3_pack(struct Buffer_info *b,  string arr[5][3]) {
  for (int i=0; i<5; i++) {
    for (int j=0; j<3; j++) {
      string_pack(b, arr[i][j]);
    }
  }
}

void string_3_pack(struct Buffer_info *b,  string arr[3]) {
  for (int i=0; i<3; i++) {
    string_pack(b, arr[i]);
  }
}

void Person_pack(struct Buffer_info *b, Person p) {
  string_3_pack(b, p.first_middle_last_names);
  float_pack(b, p.magic_num);
  int_pack(b, p.age);
}

void Person_5_pack(struct Buffer_info *b, Person arr[5]) {
  for (int i=0; i<5; i++) {
    Person_pack(b, arr[i]);
  }
}

void Info_pack(struct Buffer_info *b, Info i1) {
  string_5_3_pack(b, i1.names);
  Person_5_pack(b, i1.people);
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


string string_handler() {
  int str_len = int_handler();

  if (str_len > 0) {
    char buf[str_len];
    int n = RPCPROXYSOCKET->read(buf, str_len);

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


float float_handler() {
  string s = string_handler();
  if (s.compare("") == 0) {
    // eof on the previous string read
    return -1;
  } else {
    return atof(s.c_str());
  }
}

void string_5_3_handler(string arr[5][3]) {
  for (int i=0; i<5; i++) {
    for (int j=0; j<3; j++) {
      arr[i][j] = string_handler();
    }
  }
}

void string_3_handler(string arr[3]) {
  for (int i=0; i<3; i++) {
    arr[i] = string_handler();
  }
}

Person Person_handler() {
  Person p1;
  string_3_handler(p1.first_middle_last_names);
  p1.magic_num = float_handler();
  p1.age = int_handler();
  return p1;
}

void Person_5_handler(Person arr[5]) {
  for (int i=0; i<5; i++) {
    arr[i] = Person_handler();
  }
}

Info Info_handler() {
  Info i1;
  string_5_3_handler(i1.names);
  Person_5_handler(i1.people);
  return i1;
}


///////////////////////////////
//    call functions (remote)
///////////////////////////////

Info echo(Info x, Info y) {
  // construct msg
  struct Buffer_info b;
  b.buf = (char*) malloc(10);
  b.buf_len = 0;

  // pack in function name and args
  string_pack(&b, "echo");
  Info_pack(&b, x);
  Info_pack(&b, y);
  
  // send the function call
  RPCPROXYSOCKET->write(b.buf, b.buf_len);

  // read and return result
  Info res = Info_handler();
  return res;
}

Person echo_person (Person x, Person y) {
  // construct msg
  struct Buffer_info b;
  b.buf = (char*) malloc(10);
  b.buf_len = 0;

  // pack in function name and args
  string_pack(&b, "echo_person");
  Person_pack(&b, x);
  Person_pack(&b, y);
  
  // send the function call
  RPCPROXYSOCKET->write(b.buf, b.buf_len);

  // read and return result
  Person res = Person_handler();
  return res;
}

