//arithmetic.stub.cpp
//authors: Jialu Wei, Remmy Chen

#include <string>
using namespace std;
#include "arithmetic.idl"
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

void int_serializer(struct Buffer_info *b, int i);

int int_deserializer();

void string_serializer(struct Buffer_info *b, string s);

string string_deserializer();

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

int int_deserializer() {
	char buf[INT_LEN];
	int res;
	int readlen = RPCSTUBSOCKET->read(buf, INT_LEN);
	if (readlen == 0) return 0;
	memcpy(&res, buf, INT_LEN);
	return ntohl(res);
}
string string_deserializer() {
	int str_len = int_deserializer();
	char buf[str_len];
	int readlen = RPCSTUBSOCKET->read(buf, str_len);
	if (readlen == 0) return "";
	string res(buf, str_len);
	return res;
}

void __subtract(int x, int y) {
	int res = subtract(x, y);
	struct Buffer_info b;
	b.buf = (char*) malloc(1);
	b.buf_len = 0;
	int_serializer(&b, res);
	RPCSTUBSOCKET->write(b.buf, b.buf_len);
}

void __divide(int x, int y) {
	int res = divide(x, y);
	struct Buffer_info b;
	b.buf = (char*) malloc(1);
	b.buf_len = 0;
	int_serializer(&b, res);
	RPCSTUBSOCKET->write(b.buf, b.buf_len);
}

void __add(int x, int y) {
	int res = add(x, y);
	struct Buffer_info b;
	b.buf = (char*) malloc(1);
	b.buf_len = 0;
	int_serializer(&b, res);
	RPCSTUBSOCKET->write(b.buf, b.buf_len);
}

void __multiply(int x, int y) {
	int res = multiply(x, y);
	struct Buffer_info b;
	b.buf = (char*) malloc(1);
	b.buf_len = 0;
	int_serializer(&b, res);
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
		else if (func_name.compare("subtract")==0) {
			int x = int_deserializer();
			int y = int_deserializer();
			__subtract(x, y);
		}
		else if (func_name.compare("divide")==0) {
			int x = int_deserializer();
			int y = int_deserializer();
			__divide(x, y);
		}
		else if (func_name.compare("add")==0) {
			int x = int_deserializer();
			int y = int_deserializer();
			__add(x, y);
		}
		else if (func_name.compare("multiply")==0) {
			int x = int_deserializer();
			int y = int_deserializer();
			__multiply(x, y);
		}
		else {
			printf("BAD FUNCTION");
		}
	}
}

