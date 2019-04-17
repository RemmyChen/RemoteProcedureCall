#include <string>
using namespace std;
#include "structs.idl"   // include the idl file

#include <iostream>
#include <cstdio>

Person findPerson(ThreePeople tp) {
    printf("findPerson invoked\n");
    return tp.p2;
}

int area(rectangle r) {
    printf("area invoked\n");
    return r.x * r.y;
}
