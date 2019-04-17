//testarray2.proxy.cpp
//authors: Jialu Wei, Remmy Chen

#include <string>
using namespace std;
#include "testarray2.idl"
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

void __int_15__serializer(struct Buffer_info *b, int arr[15]);

void __int_15__deserializer(int arr[15]);

void __int_24__serializer(struct Buffer_info *b, int arr[24]);

void __int_24__deserializer(int arr[24]);

void string_serializer(struct Buffer_info *b, string s);

string string_deserializer();

void __int_24_15__serializer(struct Buffer_info *b, int arr[24][15]);

void __int_24_15__deserializer(int arr[24][15]);

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
void __int_15__serializer(struct Buffer_info *b, int arr[15]) {
	for (int i0 = 0; i0 < 15; i0++) {
		int_serializer(b, arr[i0]);
	}
}
void __int_24__serializer(struct Buffer_info *b, int arr[24]) {
	for (int i0 = 0; i0 < 24; i0++) {
		int_serializer(b, arr[i0]);
	}
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
void __int_24_15__serializer(struct Buffer_info *b, int arr[24][15]) {
	for (int i0 = 0; i0 < 24; i0++) {
		for (int i1 = 0; i1 < 15; i1++) {
			int_serializer(b, arr[i0][i1]);
		}
}
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
void __int_15__deserializer(int arr[15]) {
	for (int i0 = 0; i0 < 15; i0++) {
		arr[i0] = int_deserializer();
	}
}
void __int_24__deserializer(int arr[24]) {
	for (int i0 = 0; i0 < 24; i0++) {
		arr[i0] = int_deserializer();
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
void __int_24_15__deserializer(int arr[24][15]) {
	for (int i0 = 0; i0 < 24; i0++) {
		for (int i1 = 0; i1 < 15; i1++) {
			arr[i0][i1] = int_deserializer();
		}
	}
}

int sqrt(int x[24], int y[24][15], int z[24][15]) {
	struct Buffer_info b;
	b.buf = (char*) malloc(1);
	b.buf_len = 0;
	string_serializer(&b, "sqrt");
	__int_24__serializer(&b, x);
	__int_24_15__serializer(&b, y);
	__int_24_15__serializer(&b, z);
	RPCPROXYSOCKET->write(b.buf, b.buf_len);
	int res = int_deserializer();
	return res;
}

