///////////////////////////////////////////
//
//    test1client.cpp
//    author: Jialu Wei, Remmy Chen      
//   
///////////////////////////////////////////

#include <string>
using namespace std;
#include "test1.idl"

#include "rpcproxyhelper.h"
#include "c150debug.h"
#include "c150grading.h"
#include <fstream>
#include <iostream>
#include <cstdio>
using namespace C150NETWORK;  // for all the comp150 utilities 

// command line argument
const int serverArg = 1;

// forward declarations
void print_info(Info info);
void print_person(Person p);

// main
int main(int argc, char *argv[]) {
    // check command line args
    if (argc != 2) {
      fprintf(stderr,"Correct syntxt is: %s <servername> \n", argv[0]);
      exit(1);
    }
     
    // todo: uncomment this later
    // GRADEME(argc, argv);
    
    try {
      // set up socket
      rpcproxyinitialize(argv[serverArg]);

      Info info_to_send;

      string names[5][3] = {{"Sydney", "Warren", "Holway"}, {"Jialu", "", "Wei"}, {"Wolfgang", "Amadeus", "Mozart"}, {"Alexander", "Graham", "Bell"}, {"Ludwig", "van", "Beethoven"}};
      for (int i=0; i<5; i++) {
        for (int j=0; j<3; j++) {
          info_to_send.names[i][j] = names[i][j];
        }
      }

      float magic_nums[5] = {1996.38, 1995.811, 1756.127, 1847.33, 1827.326};
      int ages[5] = {23, 24, 35, 75, 56};
      for (int i=0; i<5; i++) {
        for (int j=0; j<3; j++) {
          info_to_send.people[i].first_middle_last_names[j] = names[i][j];
        }
        info_to_send.people[i].magic_num = magic_nums[i];
        info_to_send.people[i].age = ages[i];
      }

      // print_info(info_to_send);

      // call 
      Info info_echoed = echo(info_to_send, info_to_send);
      cout << "===================== print echoed info =====================" << endl;
      print_info(info_echoed);


      // call 
      Info info_echoed2 = echo(info_to_send, info_to_send);
      cout << "===================== print echoed info =====================" << endl;
      print_info(info_echoed2);

      // call
      Person person_echoed = echo_person(info_to_send.people[0], info_to_send.people[1]);
      cout << "===================== print echoed person =====================" << endl;
      print_person(person_echoed);

     } catch (C150Exception e) {
        // exception handle
        printf("test1client: Caught C150Exception: %s\n", e.formattedExplanation().c_str());
     }
     return 0;
}


void print_info(Info info) {
  // print "names"
  for (int i=0; i<5; i++) {
    cout << "name" << i << endl;
    for (int j=0; j<3; j++) {
      cout << info.names[i][j] << endl;
    }
    cout << endl;
  }
  
  // print "people"
  for (int i=0; i<5; i++) {
    cout << "person" << i << endl;
    print_person(info.people[i]);
    cout << endl;
  }
}

void print_person(Person p) {
  for (int j=0; j<3; j++) {
      cout << p.first_middle_last_names[j] << endl;
  }
  cout << p.magic_num << endl << p.age << endl;
}
