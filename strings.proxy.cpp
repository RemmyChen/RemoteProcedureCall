//strings.proxy.cpp
//authors: Jialu Wei, Remmy Chen

#include <string>
using namespace std;
#include "strings.idl"
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
string string_deserializer() {
	int str_len = int_deserializer();
	if (str_len > 0) {
		char buf[str_len];
		int readlen = RPCPROXYSOCKET->read(buf, str_len);
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

string concat3(string s, string t, string u) {
	struct Buffer_info b;
	b.buf = (char*) malloc(1);
	b.buf_len = 0;
	string_serializer(&b, "concat3");
	string_serializer(&b, s);
	string_serializer(&b, t);
	string_serializer(&b, u);
	RPCPROXYSOCKET->write(b.buf, b.buf_len);
	free(b.buf);
	string res = string_deserializer();
	return res;
}

string upcase(string s) {
	struct Buffer_info b;
	b.buf = (char*) malloc(1);
	b.buf_len = 0;
	string_serializer(&b, "upcase");
	string_serializer(&b, s);
	RPCPROXYSOCKET->write(b.buf, b.buf_len);
	free(b.buf);
	string res = string_deserializer();
	return res;
}

string concat(string s, string t) {
	struct Buffer_info b;
	b.buf = (char*) malloc(1);
	b.buf_len = 0;
	string_serializer(&b, "concat");
	string_serializer(&b, s);
	string_serializer(&b, t);
	RPCPROXYSOCKET->write(b.buf, b.buf_len);
	free(b.buf);
	string res = string_deserializer();
	return res;
}

