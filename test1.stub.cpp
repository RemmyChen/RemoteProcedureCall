//test1.stub.cpp
//authors: Jialu Wei, Remmy Chen

#include <string>
using namespace std;
#include "test1.idl"
#include "rpcstubhelper.h"
#include <cstdio>
#include <cstring>
#include "c150debug.h"
#include <iostream>
#include <vector>
using namespace C150NETWORK;

#define INT_LEN 4
#define FLOAT_LEN 4

struct Buffer_info {
	char *buf;
	int buf_len;
};

void __string_5__3_serializer(struct Buffer_info *b, string arr[5][3]);

void __string_5__3_deserializer(string arr[5][3]);

void __Person_5_serializer(struct Buffer_info *b, Person arr[5]);

void __Person_5_deserializer(Person arr[5]);

void Person_serializer(struct Buffer_info *b, Person P1);

Person Person_serializer();

void int_serializer(struct Buffer_info *b, int i);

int int_deserializer();

void Info_serializer(struct Buffer_info *b, Info I1);

Info Info_serializer();

void __string_3_serializer(struct Buffer_info *b, string arr[3]);

void __string_3_deserializer(string arr[3]);

void float_serializer(struct Buffer_info *b, float f);

float float_deserializer();

void string_serializer(struct Buffer_info *b, string s);

string string_deserializer();

void __string_5__3_serializer(struct Buffer_info *b, string arr[5][3]) {
	//TODO
}
void __Person_5_serializer(struct Buffer_info *b, Person arr[5]) {
	//TODO
}
void Person_serializer(struct Buffer_info *b, Person P1) {
	//TODO
}
void int_serializer(struct Buffer_info *b, int i) {
	int converted = htonl(i);
	int new_buf_len;
	char *new_buf;
	new_buf_len = b->buf_len + INT_LEN;
	new_buf = (char*) malloc(new_buf_len);
	memcpy(new_buf, b->buf, b->buf_len);
	memcpy(new_buf+b->buf_len, (char*)&converted, INT_LEN);
	free(b->buf);
	b->buf = new_buf;
	b->buf_len = new_buf_len;
}
void Info_serializer(struct Buffer_info *b, Info I1) {
	//TODO
}
void __string_3_serializer(struct Buffer_info *b, string arr[3]) {
	//TODO
}
void float_serializer(struct Buffer_info *b, float f) {
	string s = to_string(f);
	string_serializer(b, s);
}
void string_serializer(struct Buffer_info *b, string s) {
	int_serializer(b, s.length());
	int new_buf_len;
	char *new_buf;
	new_buf_len = b->buf_len + s.length();
	new_buf = (char*) malloc(new_buf_len);
	memcpy(new_buf, b->buf, b->buf_len);
	memcpy(new_buf+b->buf_len, s.c_str(), s.length());
	free(b->buf);
	b->buf = new_buf;
	b->buf_len = new_buf_len;
}

void __string_5__3_deserializer(string arr[5][3]) {
	//TODO
}
void __Person_5_deserializer(Person arr[5]) {
	//TODO
}
Person Person_serializer() {
	Person s1;
	//TODO
}
int int_deserializer() {
	char buf[INT_LEN];
	int res;
	int readlen = RPCSTUBSOCKET->read(buf, INT_LEN);
	if (readlen == 0) return 0;
	memcpy(&res, buf, INT_LEN);
	return ntohl(res);
}
Info Info_serializer() {
	Info s1;
	//TODO
}
void __string_3_deserializer(string arr[3]) {
	//TODO
}
float float_deserializer() {
	string s = string_deserializer();
	return atof(s.c_str());
}
string string_deserializer() {
	int str_len = int_deserializer();
	char buf[str_len];
	int readlen = RPCSTUBSOCKET->read(buf, str_len);
	if (readlen == 0) return "";
	string res(buf, str_len);
	return res;
}

void __echo_person(Person x, Person y) {
	Person res = echo_person(x, y);
	struct Buffer_info b;
	b.buf = (char*) malloc(1);
	b.buf_len = 0;
	Person_serializer(&b, res);
	RPCSTUBSOCKET->write(b.buf, b.buf_len);
}

void __echo(Info x, Info y) {
	Info res = echo(x, y);
	struct Buffer_info b;
	b.buf = (char*) malloc(1);
	b.buf_len = 0;
	Info_serializer(&b, res);
	RPCSTUBSOCKET->write(b.buf, b.buf_len);
}

void __badFunction(const char *functionName) {
	char doneBuffer[5] = "BAD";
	RPCSTUBSOCKET->write(doneBuffer, strlen(doneBuffer)+1);
}

void dispatchFunction() {
	if (!RPCSTUBSOCKET->eof()) {
		string func_name = string_deserializer();
		if (func_name.compare("")==0) return;
		else if (func_name.compare("echo_person")==0) {
			Person x = Person_deserializer();
			Person y = Person_deserializer();
			__echo_person(x, y);
		}
		else if (func_name.compare("echo")==0) {
			Info x = Info_deserializer();
			Info y = Info_deserializer();
			__echo(x, y);
		}
		else {
			printf("BAD FUNCTION");
		}
	}
}

