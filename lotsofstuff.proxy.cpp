//lotsofstuff.proxy.cpp
//authors: Jialu Wei, Remmy Chen

#include <string>
using namespace std;
#include "lotsofstuff.idl"
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

void __rectangle_200__serializer(struct Buffer_info *b, rectangle arr[200]);

void __rectangle_200__deserializer(rectangle arr[200]);

void ThreePeople_serializer(struct Buffer_info *b, ThreePeople T1);

ThreePeople ThreePeople_deserializer();

void MT_serializer(struct Buffer_info *b, MT M1);

MT MT_deserializer();

void __int_15__serializer(struct Buffer_info *b, int arr[15]);

void __int_15__deserializer(int arr[15]);

void string_serializer(struct Buffer_info *b, string s);

string string_deserializer();

void s_serializer(struct Buffer_info *b, s s1);

s s_deserializer();

void Person_serializer(struct Buffer_info *b, Person P1);

Person Person_deserializer();

void __int_24__serializer(struct Buffer_info *b, int arr[24]);

void __int_24__deserializer(int arr[24]);

void __int_10__serializer(struct Buffer_info *b, int arr[10]);

void __int_10__deserializer(int arr[10]);

void __int_24_15__serializer(struct Buffer_info *b, int arr[24][15]);

void __int_24_15__deserializer(int arr[24][15]);

void __int_4_10__serializer(struct Buffer_info *b, int arr[4][10]);

void __int_4_10__deserializer(int arr[4][10]);

void __int_100__serializer(struct Buffer_info *b, int arr[100]);

void __int_100__deserializer(int arr[100]);

void __int_10_100__serializer(struct Buffer_info *b, int arr[10][100]);

void __int_10_100__deserializer(int arr[10][100]);

void __Person_10__serializer(struct Buffer_info *b, Person arr[10]);

void __Person_10__deserializer(Person arr[10]);

void StructWithArrays_serializer(struct Buffer_info *b, StructWithArrays S1);

StructWithArrays StructWithArrays_deserializer();

void __int_4_10_100__serializer(struct Buffer_info *b, int arr[4][10][100]);

void __int_4_10_100__deserializer(int arr[4][10][100]);



void __int_4__serializer(struct Buffer_info *b, int arr[4]);

void __int_4__deserializer(int arr[4]);

void rectangle_serializer(struct Buffer_info *b, rectangle r1);

rectangle rectangle_deserializer();

void int_serializer(struct Buffer_info *b, int i);

int int_deserializer();

void __Person_20__serializer(struct Buffer_info *b, Person arr[20]);

void __Person_20__deserializer(Person arr[20]);

void float_serializer(struct Buffer_info *b, float f);

float float_deserializer();

void __rectangle_200__serializer(struct Buffer_info *b, rectangle arr[200]) {
	for (int i0 = 0; i0 < 200; i0++) {
		rectangle_serializer(b, arr[i0]);
	}
}
void ThreePeople_serializer(struct Buffer_info *b, ThreePeople T1) {
	Person_serializer(b, T1.p1);
	Person_serializer(b, T1.p2);
	Person_serializer(b, T1.p3);
}
void MT_serializer(struct Buffer_info *b, MT M1) {
}
void __int_15__serializer(struct Buffer_info *b, int arr[15]) {
	for (int i0 = 0; i0 < 15; i0++) {
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
	free(b->buf);
	b->buf = new_buf;
	b->buf_len = new_buf_len;
}
void s_serializer(struct Buffer_info *b, s s1) {
	__int_4__serializer(b, s1.m1);
	__int_4_10__serializer(b, s1.m2);
	__int_4_10_100__serializer(b, s1.m3);
}
void Person_serializer(struct Buffer_info *b, Person P1) {
	string_serializer(b, P1.firstname);
	string_serializer(b, P1.lastname);
	int_serializer(b, P1.age);
}
void __int_24__serializer(struct Buffer_info *b, int arr[24]) {
	for (int i0 = 0; i0 < 24; i0++) {
		int_serializer(b, arr[i0]);
	}
}
void __int_10__serializer(struct Buffer_info *b, int arr[10]) {
	for (int i0 = 0; i0 < 10; i0++) {
		int_serializer(b, arr[i0]);
	}
}
void __int_24_15__serializer(struct Buffer_info *b, int arr[24][15]) {
	for (int i0 = 0; i0 < 24; i0++) {
		for (int i1 = 0; i1 < 15; i1++) {
			int_serializer(b, arr[i0][i1]);
		}
}
}
void __int_4_10__serializer(struct Buffer_info *b, int arr[4][10]) {
	for (int i0 = 0; i0 < 4; i0++) {
		for (int i1 = 0; i1 < 10; i1++) {
			int_serializer(b, arr[i0][i1]);
		}
}
}
void __int_100__serializer(struct Buffer_info *b, int arr[100]) {
	for (int i0 = 0; i0 < 100; i0++) {
		int_serializer(b, arr[i0]);
	}
}
void __int_10_100__serializer(struct Buffer_info *b, int arr[10][100]) {
	for (int i0 = 0; i0 < 10; i0++) {
		for (int i1 = 0; i1 < 100; i1++) {
			int_serializer(b, arr[i0][i1]);
		}
}
}
void __Person_10__serializer(struct Buffer_info *b, Person arr[10]) {
	for (int i0 = 0; i0 < 10; i0++) {
		Person_serializer(b, arr[i0]);
	}
}
void StructWithArrays_serializer(struct Buffer_info *b, StructWithArrays S1) {
	int_serializer(b, S1.aNumber);
	__Person_10__serializer(b, S1.people);
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
void __int_4__serializer(struct Buffer_info *b, int arr[4]) {
	for (int i0 = 0; i0 < 4; i0++) {
		int_serializer(b, arr[i0]);
	}
}
void rectangle_serializer(struct Buffer_info *b, rectangle r1) {
	int_serializer(b, r1.x);
	int_serializer(b, r1.y);
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
void __Person_20__serializer(struct Buffer_info *b, Person arr[20]) {
	for (int i0 = 0; i0 < 20; i0++) {
		Person_serializer(b, arr[i0]);
	}
}
void float_serializer(struct Buffer_info *b, float f) {
	string s = to_string(f);
	string_serializer(b, s);
}

void __rectangle_200__deserializer(rectangle arr[200]) {
	for (int i0 = 0; i0 < 200; i0++) {
		arr[i0] = rectangle_deserializer();
	}
}
ThreePeople ThreePeople_deserializer() {
	ThreePeople T1;
	Person_deserializer(T1.p1);
	Person_deserializer(T1.p2);
	Person_deserializer(T1.p3);
	return T1;
}
MT MT_deserializer() {
	MT M1;
	return M1;
}
void __int_15__deserializer(int arr[15]) {
	for (int i0 = 0; i0 < 15; i0++) {
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
s s_deserializer() {
	s s1;
	__int_4__deserializer(s1.m1);
	__int_4_10__deserializer(s1.m2);
	__int_4_10_100__deserializer(s1.m3);
	return s1;
}
Person Person_deserializer() {
	Person P1;
	string_deserializer();
	string_deserializer();
	int_deserializer();
	return P1;
}
void __int_24__deserializer(int arr[24]) {
	for (int i0 = 0; i0 < 24; i0++) {
		arr[i0] = int_deserializer();
	}
}
void __int_10__deserializer(int arr[10]) {
	for (int i0 = 0; i0 < 10; i0++) {
		arr[i0] = int_deserializer();
	}
}
void __int_24_15__deserializer(int arr[24][15]) {
	for (int i0 = 0; i0 < 24; i0++) {
		for (int i1 = 0; i1 < 15; i1++) {
			arr[i0][i1] = int_deserializer();
		}
	}
}
void __int_4_10__deserializer(int arr[4][10]) {
	for (int i0 = 0; i0 < 4; i0++) {
		for (int i1 = 0; i1 < 10; i1++) {
			arr[i0][i1] = int_deserializer();
		}
	}
}
void __int_100__deserializer(int arr[100]) {
	for (int i0 = 0; i0 < 100; i0++) {
		arr[i0] = int_deserializer();
	}
}
void __int_10_100__deserializer(int arr[10][100]) {
	for (int i0 = 0; i0 < 10; i0++) {
		for (int i1 = 0; i1 < 100; i1++) {
			arr[i0][i1] = int_deserializer();
		}
	}
}
void __Person_10__deserializer(Person arr[10]) {
	for (int i0 = 0; i0 < 10; i0++) {
		arr[i0] = Person_deserializer();
	}
}
StructWithArrays StructWithArrays_deserializer() {
	StructWithArrays S1;
	int_deserializer();
	__Person_10__deserializer(S1.people);
	return S1;
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
void __int_4__deserializer(int arr[4]) {
	for (int i0 = 0; i0 < 4; i0++) {
		arr[i0] = int_deserializer();
	}
}
rectangle rectangle_deserializer() {
	rectangle r1;
	int_deserializer();
	int_deserializer();
	return r1;
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
void __Person_20__deserializer(Person arr[20]) {
	for (int i0 = 0; i0 < 20; i0++) {
		arr[i0] = Person_deserializer();
	}
}
float float_deserializer() {
	string s = string_deserializer();
	if (s.compare("")==0) {
		return -1;
	} else {
	return atof(s.c_str());
	}
}

Person findPersonByAge(StructWithArrays swa) {
	struct Buffer_info b;
	b.buf = (char*) malloc(1);
	b.buf_len = 0;
	string_serializer(&b, "findPersonByAge");
	StructWithArrays_serializer(&b, swa);
	RPCPROXYSOCKET->write(b.buf, b.buf_len);
	free(b.buf);
	Person res = Person_deserializer();
	return res;
}

int takesTwoArrays(__int[24] x, __int[24] y) {
	struct Buffer_info b;
	b.buf = (char*) malloc(1);
	b.buf_len = 0;
	string_serializer(&b, "takesTwoArrays");
	__int[24]_serializer(&b, x);
	__int[24]_serializer(&b, y);
	RPCPROXYSOCKET->write(b.buf, b.buf_len);
	free(b.buf);
	int res = int_deserializer();
	return res;
}

int sqrt(int x, int y) {
	struct Buffer_info b;
	b.buf = (char*) malloc(1);
	b.buf_len = 0;
	string_serializer(&b, "sqrt");
	int_serializer(&b, x);
	int_serializer(&b, y);
	RPCPROXYSOCKET->write(b.buf, b.buf_len);
	free(b.buf);
	int res = int_deserializer();
	return res;
}

void func2() {
	struct Buffer_info b;
	b.buf = (char*) malloc(1);
	b.buf_len = 0;
	string_serializer(&b, "func2");
	RPCPROXYSOCKET->write(b.buf, b.buf_len);
	free(b.buf);
	string res = string_deserializer();
	if (res.compare("DONE")!=0) {
		throw C150Exception("simplefunction.proxy: func1() received invalid response from the server");
	}
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

void searchRectangles(int area, __rectangle[200] rects) {
	struct Buffer_info b;
	b.buf = (char*) malloc(1);
	b.buf_len = 0;
	string_serializer(&b, "searchRectangles");
	int_serializer(&b, area);
	__rectangle[200]_serializer(&b, rects);
	RPCPROXYSOCKET->write(b.buf, b.buf_len);
	free(b.buf);
	string res = string_deserializer();
	if (res.compare("DONE")!=0) {
		throw C150Exception("simplefunction.proxy: func1() received invalid response from the server");
	}
}

void func3() {
	struct Buffer_info b;
	b.buf = (char*) malloc(1);
	b.buf_len = 0;
	string_serializer(&b, "func3");
	RPCPROXYSOCKET->write(b.buf, b.buf_len);
	free(b.buf);
	string res = string_deserializer();
	if (res.compare("DONE")!=0) {
		throw C150Exception("simplefunction.proxy: func1() received invalid response from the server");
	}
}

Person findPerson(ThreePeople tp) {
	struct Buffer_info b;
	b.buf = (char*) malloc(1);
	b.buf_len = 0;
	string_serializer(&b, "findPerson");
	ThreePeople_serializer(&b, tp);
	RPCPROXYSOCKET->write(b.buf, b.buf_len);
	free(b.buf);
	Person res = Person_deserializer();
	return res;
}

void func1() {
	struct Buffer_info b;
	b.buf = (char*) malloc(1);
	b.buf_len = 0;
	string_serializer(&b, "func1");
	RPCPROXYSOCKET->write(b.buf, b.buf_len);
	free(b.buf);
	string res = string_deserializer();
	if (res.compare("DONE")!=0) {
		throw C150Exception("simplefunction.proxy: func1() received invalid response from the server");
	}
}

int area(rectangle r) {
	struct Buffer_info b;
	b.buf = (char*) malloc(1);
	b.buf_len = 0;
	string_serializer(&b, "area");
	rectangle_serializer(&b, r);
	RPCPROXYSOCKET->write(b.buf, b.buf_len);
	free(b.buf);
	int res = int_deserializer();
	return res;
}

Person dealWithPersonArrays(__Person[20] people1, __Person[10] people2) {
	struct Buffer_info b;
	b.buf = (char*) malloc(1);
	b.buf_len = 0;
	string_serializer(&b, "dealWithPersonArrays");
	__Person[20]_serializer(&b, people1);
	__Person[10]_serializer(&b, people2);
	RPCPROXYSOCKET->write(b.buf, b.buf_len);
	free(b.buf);
	Person res = Person_deserializer();
	return res;
}

int showsArraysofArrays(__int[24] x, __int[24][15] y, __int[24][15] z) {
	struct Buffer_info b;
	b.buf = (char*) malloc(1);
	b.buf_len = 0;
	string_serializer(&b, "showsArraysofArrays");
	__int[24]_serializer(&b, x);
	__int[24][15]_serializer(&b, y);
	__int[24][15]_serializer(&b, z);
	RPCPROXYSOCKET->write(b.buf, b.buf_len);
	free(b.buf);
	int res = int_deserializer();
	return res;
}

