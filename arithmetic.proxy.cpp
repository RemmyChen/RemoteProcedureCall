//arithmetic.proxy.cpp
//authors: Jialu Wei, Remmy Chen

#include <string>
using namespace std;
#include "arithmetic.idl"
#include "rpcproxyhelper.h"
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
	int readlen = RPCPROXYSOCKET->read(buf, INT_LEN);
	if (readlen == 0) return 0;
	memcpy(&res, buf, INT_LEN);
	return ntohl(res);
}
string string_deserializer() {
	int str_len = int_deserializer();
	char buf[str_len];
	int readlen = RPCPROXYSOCKET->read(buf, str_len);
	if (readlen == 0) return "";
	string res(buf, str_len);
	return res;
}

int subtract(int x, int y) {
	struct Buffer_info b;
	b.buf = (char*) malloc(1);
	b.buf_len = 0;
	string_serializer(&b, "subtract");
	int_serializer(&b, x);
	int_serializer(&b, y);
	RPCPROXYSOCKET->write(b.buf, b.buf_len);
	free(b.buf);
	int res = int_deserializer();
	return res;
}

int divide(int x, int y) {
	struct Buffer_info b;
	b.buf = (char*) malloc(1);
	b.buf_len = 0;
	string_serializer(&b, "divide");
	int_serializer(&b, x);
	int_serializer(&b, y);
	RPCPROXYSOCKET->write(b.buf, b.buf_len);
	free(b.buf);
	int res = int_deserializer();
	return res;
}

int add(int x, int y) {
	struct Buffer_info b;
	b.buf = (char*) malloc(1);
	b.buf_len = 0;
	string_serializer(&b, "add");
	int_serializer(&b, x);
	int_serializer(&b, y);
	RPCPROXYSOCKET->write(b.buf, b.buf_len);
	free(b.buf);
	int res = int_deserializer();
	return res;
}

int multiply(int x, int y) {
	struct Buffer_info b;
	b.buf = (char*) malloc(1);
	b.buf_len = 0;
	string_serializer(&b, "multiply");
	int_serializer(&b, x);
	int_serializer(&b, y);
	RPCPROXYSOCKET->write(b.buf, b.buf_len);
	free(b.buf);
	int res = int_deserializer();
	return res;
}

