//structwitharray.proxy.cpp
//authors: Jialu Wei, Remmy Chen

#include <string>
using namespace std;
#include "structwitharray.idl"
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

void __int_4__serializer(struct Buffer_info *b, int arr[4]);

void __int_4__deserializer(int arr[4]);

void __int_4_10__serializer(struct Buffer_info *b, int arr[4][10]);

void __int_4_10__deserializer(int arr[4][10]);

void __int_10__serializer(struct Buffer_info *b, int arr[10]);

void __int_10__deserializer(int arr[10]);

void __int_100__serializer(struct Buffer_info *b, int arr[100]);

void __int_100__deserializer(int arr[100]);

void s_serializer(struct Buffer_info *b, s s1);

void s_deserializer(s *s1);

void int_serializer(struct Buffer_info *b, int i);

int int_deserializer();

void string_serializer(struct Buffer_info *b, string s);

string string_deserializer();

void __int_10_100__serializer(struct Buffer_info *b, int arr[10][100]);

void __int_10_100__deserializer(int arr[10][100]);

void __int_4_10_100__serializer(struct Buffer_info *b, int arr[4][10][100]);

void __int_4_10_100__deserializer(int arr[4][10][100]);

void __int_4__serializer(struct Buffer_info *b, int arr[4]) {
	for (int i0 = 0; i0 < 4; i0++) {
		int_serializer(b, arr[i0]);
	}
}
void __int_4_10__serializer(struct Buffer_info *b, int arr[4][10]) {
	for (int i0 = 0; i0 < 4; i0++) {
		for (int i1 = 0; i1 < 10; i1++) {
			int_serializer(b, arr[i0][i1]);
		}
}
}
void __int_10__serializer(struct Buffer_info *b, int arr[10]) {
	for (int i0 = 0; i0 < 10; i0++) {
		int_serializer(b, arr[i0]);
	}
}
void __int_100__serializer(struct Buffer_info *b, int arr[100]) {
	for (int i0 = 0; i0 < 100; i0++) {
		int_serializer(b, arr[i0]);
	}
}
void s_serializer(struct Buffer_info *b, s s1) {
	__int_4__serializer(b, s1.m1);
	__int_4_10__serializer(b, s1.m2);
	__int_4_10_100__serializer(b, s1.m3);
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
void __int_10_100__serializer(struct Buffer_info *b, int arr[10][100]) {
	for (int i0 = 0; i0 < 10; i0++) {
		for (int i1 = 0; i1 < 100; i1++) {
			int_serializer(b, arr[i0][i1]);
		}
}
}
void __int_4_10_100__serializer(struct Buffer_info *b, int arr[4][10][100]) {
	for (int i0 = 0; i0 < 4; i0++) {
		for (int i1 = 0; i1 < 10; i1++) {
			for (int i2 = 0; i2 < 100; i2++) {
				int_serializer(b, arr[i0][i1][i2]);
			}
	}
}
}

void __int_4__deserializer(int arr[4]) {
	for (int i0 = 0; i0 < 4; i0++) {
		arr[i0] = int_deserializer();
	}
}
void __int_4_10__deserializer(int arr[4][10]) {
	for (int i0 = 0; i0 < 4; i0++) {
		for (int i1 = 0; i1 < 10; i1++) {
			arr[i0][i1] = int_deserializer();
		}
	}
}
void __int_10__deserializer(int arr[10]) {
	for (int i0 = 0; i0 < 10; i0++) {
		arr[i0] = int_deserializer();
	}
}
void __int_100__deserializer(int arr[100]) {
	for (int i0 = 0; i0 < 100; i0++) {
		arr[i0] = int_deserializer();
	}
}
void s_deserializer(s *s1) {
	__int_4__deserializer(s1->m1);
	__int_4_10__deserializer(s1->m2);
	__int_4_10_100__deserializer(s1->m3);
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
void __int_10_100__deserializer(int arr[10][100]) {
	for (int i0 = 0; i0 < 10; i0++) {
		for (int i1 = 0; i1 < 100; i1++) {
			arr[i0][i1] = int_deserializer();
		}
	}
}
void __int_4_10_100__deserializer(int arr[4][10][100]) {
	for (int i0 = 0; i0 < 4; i0++) {
		for (int i1 = 0; i1 < 10; i1++) {
			for (int i2 = 0; i2 < 100; i2++) {
				arr[i0][i1][i2] = int_deserializer();
			}
		}
	}
}

s echo_s(s vl1, s vl2) {
	struct Buffer_info b;
	b.buf = (char*) malloc(1);
	b.buf_len = 0;
	string_serializer(&b, "echo_s");
	s_serializer(&b, vl1);
	s_serializer(&b, vl2);
	RPCPROXYSOCKET->write(b.buf, b.buf_len);
	s res;
	s_deserializer(&res);
	return res;
}

