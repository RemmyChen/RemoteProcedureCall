//floatarithmetic.proxy.cpp
//authors: Jialu Wei, Remmy Chen

#include <string>
using namespace std;
#include "floatarithmetic.idl"
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

void float_serializer(struct Buffer_info *b, float f);

float float_deserializer();

void int_serializer(struct Buffer_info *b, int i);

int int_deserializer();

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

float float_deserializer() {
	string s = string_deserializer();
	return atof(s.c_str());
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

float add(float x, float y) {
	struct Buffer_info b;
	b.buf = (char*) malloc(1);
	b.buf_len = 0;
	string_serializer(&b, "add");
	float_serializer(&b, x);
	float_serializer(&b, y);
	RPCPROXYSOCKET->write(b.buf, b.buf_len);
	free(b.buf);
	float res = float_deserializer();
	return res;
}

float subtract(float x, float y) {
	struct Buffer_info b;
	b.buf = (char*) malloc(1);
	b.buf_len = 0;
	string_serializer(&b, "subtract");
	float_serializer(&b, x);
	float_serializer(&b, y);
	RPCPROXYSOCKET->write(b.buf, b.buf_len);
	free(b.buf);
	float res = float_deserializer();
	return res;
}

float multiply(float x, float y) {
	struct Buffer_info b;
	b.buf = (char*) malloc(1);
	b.buf_len = 0;
	string_serializer(&b, "multiply");
	float_serializer(&b, x);
	float_serializer(&b, y);
	RPCPROXYSOCKET->write(b.buf, b.buf_len);
	free(b.buf);
	float res = float_deserializer();
	return res;
}

float divide(float x, float y) {
	struct Buffer_info b;
	b.buf = (char*) malloc(1);
	b.buf_len = 0;
	string_serializer(&b, "divide");
	float_serializer(&b, x);
	float_serializer(&b, y);
	RPCPROXYSOCKET->write(b.buf, b.buf_len);
	free(b.buf);
	float res = float_deserializer();
	return res;
}

