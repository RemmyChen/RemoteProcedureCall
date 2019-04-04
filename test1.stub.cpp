///////////////////////////////////////////
//
//    test1.stub.cpp 
//   
///////////////////////////////////////////

#include <string>
using namespace std;
#include "test1.idl"   // include the idl file

#include "rpcstubhelper.h"
#include <cstdio>
#include <cstring>
#include "c150debug.h"
#include <vector>
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

void float_pack(struct Buffer_info *b,  float f1) {
  float converted = htonl(f1);
  int new_buf_len; 
  char *new_buf;

  new_buf_len= b->buf_len + FLOAT_LEN;
  new_buf = (char*)malloc(new_buf_len);
  memcpy(new_buf, b->buf, b->buf_len);
  memcpy(new_buf+b->buf_len, (char *)&converted, FLOAT_LEN);
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
  int readlen = RPCSTUBSOCKET->read(buf, INT_LEN);
  if (readlen == 0) return 0;
  memcpy(&res, buf, INT_LEN);
  return ntohl(res);
}


float float_handler() {
  char buf[FLOAT_LEN];
  float res;
  int readlen = RPCSTUBSOCKET->read(buf, FLOAT_LEN);
  if (readlen == 0) return 0;
  memcpy(&res, buf, FLOAT_LEN);
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
//    call functions (local)
///////////////////////////////

void __echo(Info x, Info y) {
  Info res = echo(x, y);

  struct Buffer_info b;
  b.buf = (char*) malloc(10);
  b.buf_len = 0;
  Info_pack(&b, res);
  RPCSTUBSOCKET->write(b.buf, b.buf_len);
  free(b.buf); //  
}


void __echo_person(Person x, Person y) {
  Person res = echo_person(x, y);

  struct Buffer_info b;
  b.buf = (char*) malloc(10);
  b.buf_len = 0;
  Person_pack(&b, res);
  RPCSTUBSOCKET->write(b.buf, b.buf_len);
  free(b.buf); //  
}


///////////////////////////////
//    dispatch function
///////////////////////////////
void dispatchFunction() {
  printf("IN dispatchFunction\n");

  if (!RPCSTUBSOCKET -> eof()) {
    string func_name = string_handler();
    if (func_name.compare("")==0) {
        return;
    }
    if (func_name.compare("echo")==0) {
      Info x = Info_handler();
      Info y = Info_handler();
      __echo(x, y);
    } else if (func_name.compare("echo_person")==0) {
      Person x = Person_handler();
      Person y = Person_handler();
      __echo_person(x, y);
    } else {
      printf("BAD function\n");
      // need to call some handler to read in the rest 
    }
  }

}
