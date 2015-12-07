#include <iostream>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <vector>
#include <stdio.h> //popen
#include <sys/stat.h>
#include <fstream>

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

bool fexists(string path) {
    struct stat buffer;
    return (stat(path.c_str(), &buffer) == 0);
}

int filesize(string path) {
    ifstream in(path, ifstream::ate | ifstream::binary);
    return in.tellg();
}
