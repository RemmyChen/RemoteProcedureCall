//test.stub.cpp
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

#define BUFSIZE 1024

void __concat3(string s, string t, string u) {
	string res = concat3(s, t, u);
	//TODO
}

void __upcase(string s) {
	string res = upcase(s);
	//TODO
}

void __concat(string s, string t) {
	string res = concat(s, t);
	//TODO
}

void __badFunction(const char *functionName) {
	char doneBuffer[5] = "BAD";
	RPCSTUBSOCKET->write(doneBuffer, strlen(doneBuffer)+1);
}

void dispatchFunction() {
	//TODO
}

