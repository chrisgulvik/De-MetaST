/************************************************
  Copyright (c) 2012, The University of Tennessee Research Foundation
  All Rights Reserved.

  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

  Redistributions of source code must retain the above copyright notice, this list of conditions and the following   disclaimer.

  Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 ***************************************************/

#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <sys/wait.h>
#include <stdio.h>
#include <fcntl.h>
#include <fstream>
#include <string>
#include <time.h>
#include "Search.h"
#include "Primer.h"

using namespace std;

#define max_date 15
int main(int argc, char ** argv){


  bool DEFAULT;

  DEFAULT = false;
  char * args[14];
  int rv(0), pid, status;
  int fd2;
  fstream ifs;
  string fn;
  string Forward, Reverse;
  vector <Primer_set *> primers;
  Primer_set *p;
  char buff[100];
  time_t the_time;
  char date[max_date];

  date[0] = '\0';

  the_time = time(NULL);
  if(the_time != -1){
    strftime(date, max_date, "%d%b%y_%H:%M", localtime(&the_time));
  }

  if(argc < 2){
    fprintf(stderr, "USAGE: De-MetaST Databases ...\n");
    exit(1);
  }


  cout << "Input Filename for Primers (press Enter to use Primers.txt)\n";
  getline(cin, fn);
  if(fn == "NONE"){
    p = new Primer_set;
    cout << "Input Forward Primer (5' to 3')" << endl;
    cin >> Forward;
    cout << "Input Reverse Primer (5' to 3')" << endl;
    cin >> Reverse;
    cout << "Forward Primer: " << Forward << endl;
    cout << "Reverse Primer: " << Reverse << endl;
    primers.push_back(p);
    p->forward = Forward;
    p->reverse = Reverse;
    PrimerSearch(primers, argc-1, ++argv);
  }
  else{
    if(fn == ""){
      fn = "Primers.txt";
    }
    printf("Opening: %s\n", fn.c_str());
    ifs.open(fn.c_str());
    if(ifs.fail()){
      perror(fn.c_str());
    }
    while(getline(ifs, Forward)){
      if(Forward != ""){
        p = new Primer_set;
        primers.push_back(p);

        getline(ifs, Reverse);
        p->forward = Forward;
        p->reverse = Reverse;

        cout << "Primer Set: " << primers.size() << endl;
        cout << "Forward Primer: " << p->forward << endl;
        cout << "Reverse Primer: " << p->reverse << endl << endl;

      }
    }
    for(int i = 0; i < primers.size(); i++){
      p = primers[i];
      sprintf(buff, "De-MetaST_hits_%s(%dof%d).fa",date,  i+1, (int) primers.size());
      p->ofs = fopen(buff, "w");
      p->blast_hits = buff;
      sprintf(buff, "non-blasted_De-MetaST_hits_%s(%dof%d).fa",date,  i+1, (int) primers.size());
      p->non_ofs = fopen(buff, "w");
      p->non_blasted = buff;
    }
    PrimerSearch(primers, argc-1, argv+1);
  }

}
