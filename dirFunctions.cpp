#include <iostream>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <vector>
#include <stdio.h> //popen

using namespace std;

void cd(string path){
    if(chdir(path.c_str()) != 0){
        cout << "Error: could not change directory." << endl;
        cerr << strerror(errno) << endl;
    }
}

string ls(string args){
    string output;
    args += " 2>&1";
    FILE *lsOut = popen(args.c_str(), "r");
    if(!lsOut){
        throw "Couldn't execute ls";
    }
    char buffer[1024];
    while(fgets(buffer, sizeof(buffer), lsOut)!=NULL){
        output += buffer;
    }
    pclose(lsOut);
    return output;
}