//structs.proxy.cpp
//authors: Jialu Wei, Remmy Chen

#include <string>
using namespace std;
#include "structs.idl"
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

void ThreePeople_serializer(struct Buffer_info *b, ThreePeople T1);

void ThreePeople_deserializer(ThreePeople *T1);

void int_serializer(struct Buffer_info *b, int i);

int int_deserializer();

void StructWithArrays_serializer(struct Buffer_info *b, StructWithArrays S1);

void StructWithArrays_deserializer(StructWithArrays *S1);

void Person_serializer(struct Buffer_info *b, Person P1);

void Person_deserializer(Person *P1);

void __Person_10__serializer(struct Buffer_info *b, Person arr[10]);

void __Person_10__deserializer(Person arr[10]);

void string_serializer(struct Buffer_info *b, string s);

string string_deserializer();

void rectangle_serializer(struct Buffer_info *b, rectangle r1);

void rectangle_deserializer(rectangle *r1);

void ThreePeople_serializer(struct Buffer_info *b, ThreePeople T1) {
	Person_serializer(b, T1.p1);
	Person_serializer(b, T1.p2);
	Person_serializer(b, T1.p3);
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
void StructWithArrays_serializer(struct Buffer_info *b, StructWithArrays S1) {
	int_serializer(b, S1.aNumber);
	__Person_10__serializer(b, S1.people);
}
void Person_serializer(struct Buffer_info *b, Person P1) {
	string_serializer(b, P1.firstname);
	string_serializer(b, P1.lastname);
	int_serializer(b, P1.age);
}
void __Person_10__serializer(struct Buffer_info *b, Person arr[10]) {
	for (int i0 = 0; i0 < 10; i0++) {
		Person_serializer(b, arr[i0]);
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
void rectangle_serializer(struct Buffer_info *b, rectangle r1) {
	int_serializer(b, r1.x);
	int_serializer(b, r1.y);
}

void ThreePeople_deserializer(ThreePeople *T1) {
	Person_deserializer(&(T1->p1));
	Person_deserializer(&(T1->p2));
	Person_deserializer(&(T1->p3));
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
void StructWithArrays_deserializer(StructWithArrays *S1) {
	S1->aNumber = int_deserializer();
	__Person_10__deserializer(S1->people);
}
void Person_deserializer(Person *P1) {
	P1->firstname = string_deserializer();
	P1->lastname = string_deserializer();
	P1->age = int_deserializer();
}
void __Person_10__deserializer(Person arr[10]) {
	for (int i0 = 0; i0 < 10; i0++) {
		Person_deserializer(&(arr[i0]));
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
void rectangle_deserializer(rectangle *r1) {
	r1->x = int_deserializer();
	r1->y = int_deserializer();
}

Person findPerson(ThreePeople tp) {
	struct Buffer_info b;
	b.buf = (char*) malloc(1);
	b.buf_len = 0;
	string_serializer(&b, "findPerson");
	ThreePeople_serializer(&b, tp);
	RPCPROXYSOCKET->write(b.buf, b.buf_len);
	Person res;
	Person_deserializer(&res);
	return res;
}

int area(rectangle r) {
	struct Buffer_info b;
	b.buf = (char*) malloc(1);
	b.buf_len = 0;
	string_serializer(&b, "area");
	rectangle_serializer(&b, r);
	RPCPROXYSOCKET->write(b.buf, b.buf_len);
	int res = int_deserializer();
	return res;
}

