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

void float_serializer(struct Buffer_info *b, float f);

float float_deserializer();

void int_serializer(struct Buffer_info *b, int i);

int int_deserializer();

void __Person_5__serializer(struct Buffer_info *b, Person arr[5]);

void __Person_5__deserializer(Person arr[5]);

void __string_3__serializer(struct Buffer_info *b, string arr[3]);

void __string_3__deserializer(string arr[3]);

void Info_serializer(struct Buffer_info *b, Info I1);

void Info_deserializer(Info *I1);

void __string_5_3__serializer(struct Buffer_info *b, string arr[5][3]);

void __string_5_3__deserializer(string arr[5][3]);

void Person_serializer(struct Buffer_info *b, Person P1);

void Person_deserializer(Person *P1);

void string_serializer(struct Buffer_info *b, string s);

string string_deserializer();

void float_serializer(struct Buffer_info *b, float f) {
	string s = to_string(f);
	string_serializer(b, s);
}
void int_serializer(struct Buffer_info *b, int i) {
	int converted = htonl(i);
	int new_buf_len;
	char *new_buf;
	new_buf_len = b->buf_len + INT_LEN;
	new_buf = (char*) malloc(new_buf_len);
	memcpy(new_buf, b->buf, b->buf_len);
	memcpy(new_buf+b->buf_len, (char*)&converted, INT_LEN);
	b->buf = new_buf;
	b->buf_len = new_buf_len;
}
void __Person_5__serializer(struct Buffer_info *b, Person arr[5]) {
	for (int i0 = 0; i0 < 5; i0++) {
		Person_serializer(b, arr[i0]);
	}
}
void __string_3__serializer(struct Buffer_info *b, string arr[3]) {
	for (int i0 = 0; i0 < 3; i0++) {
		string_serializer(b, arr[i0]);
	}
}
void Info_serializer(struct Buffer_info *b, Info I1) {
	__string_5_3__serializer(b, I1.names);
	__Person_5__serializer(b, I1.people);
}
void __string_5_3__serializer(struct Buffer_info *b, string arr[5][3]) {
	for (int i0 = 0; i0 < 5; i0++) {
		for (int i1 = 0; i1 < 3; i1++) {
			string_serializer(b, arr[i0][i1]);
		}
}
}
void Person_serializer(struct Buffer_info *b, Person P1) {
	__string_3__serializer(b, P1.first_middle_last_names);
	float_serializer(b, P1.magic_num);
	int_serializer(b, P1.age);
}
void string_serializer(struct Buffer_info *b, string s) {
	int_serializer(b, s.length());
	int new_buf_len;
	char *new_buf;
	new_buf_len = b->buf_len + s.length();
	new_buf = (char*) malloc(new_buf_len);
	memcpy(new_buf, b->buf, b->buf_len);
	memcpy(new_buf+b->buf_len, s.c_str(), s.length());
	b->buf = new_buf;
	b->buf_len = new_buf_len;
}

float float_deserializer() {
	string s = string_deserializer();
	if (s.compare("")==0) {
		return -1;
	} else {
	return atof(s.c_str());
	}
}
int int_deserializer() {
	char buf[INT_LEN];
	int res;
	int readlen = RPCSTUBSOCKET->read(buf, INT_LEN);
	if (readlen == 0) {
		return -1;
	} else if (readlen < 0) {
		perror("readerror");
		exit(1);
	} else {
		memcpy(&res, buf, INT_LEN);
		return ntohl(res);
	}
}
void __Person_5__deserializer(Person arr[5]) {
	for (int i0 = 0; i0 < 5; i0++) {
		Person_deserializer(&(arr[i0]));
	}
}
void __string_3__deserializer(string arr[3]) {
	for (int i0 = 0; i0 < 3; i0++) {
		arr[i0] = string_deserializer();
	}
}
void Info_deserializer(Info *I1) {
	__string_5_3__deserializer(I1->names);
	__Person_5__deserializer(I1->people);
}
void __string_5_3__deserializer(string arr[5][3]) {
	for (int i0 = 0; i0 < 5; i0++) {
		for (int i1 = 0; i1 < 3; i1++) {
			arr[i0][i1] = string_deserializer();
		}
	}
}
void Person_deserializer(Person *P1) {
	__string_3__deserializer(P1->first_middle_last_names);
	P1->magic_num = float_deserializer();
	P1->age = int_deserializer();
}
string string_deserializer() {
	int str_len = int_deserializer();
	if (str_len > 0) {
		char buf[str_len];
		int readlen = RPCSTUBSOCKET->read(buf, str_len);
		if (readlen == 0) {
			return "";
		} else if (readlen < 0) {
			perror("read error");
			exit(1);
		} else {
			string res(buf, str_len);
			return res;
		}
	} else {
		return "";
	}
}

void __echo(Info x, Info y) {
	Info res = echo(x, y);
	struct Buffer_info b;
	b.buf = (char*) malloc(1);
	b.buf_len = 0;
	Info_serializer(&b, res);
	RPCSTUBSOCKET->write(b.buf, b.buf_len);
}

void __echo_person(Person x, Person y) {
	Person res = echo_person(x, y);
	struct Buffer_info b;
	b.buf = (char*) malloc(1);
	b.buf_len = 0;
	Person_serializer(&b, res);
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
		else if (func_name.compare("echo")==0) {
			Info x;
			Info_deserializer(&(x));
			Info y;
			Info_deserializer(&(y));
			__echo(x, y);
		}
		else if (func_name.compare("echo_person")==0) {
			Person x;
			Person_deserializer(&(x));
			Person y;
			Person_deserializer(&(y));
			__echo_person(x, y);
		}
		else {
			printf("BAD FUNCTION");
		}
	}
}

