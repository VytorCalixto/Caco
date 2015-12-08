#include <iostream>
#include <fstream>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <vector>
#include <stdio.h> //popen
#include <sys/stat.h>
#include <fstream>
#include <sys/statvfs.h>

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

string getWorkingPath(){
   char temp[1024];
   return (getcwd(temp, 1024) ? string(temp) : string(""));
}

bool fexists(string path) {
    struct stat buffer;
    return (stat(path.c_str(), &buffer) == 0);
}

int filesize(string path) {
    ifstream in(path, ifstream::ate | ifstream::binary);
    return in.tellg();
}

bool hasEnoughSpace(int size){
    struct statvfs fsData;
    string path = getWorkingPath();
    statvfs(path.c_str(), &fsData);
    long long freeSpace = fsData.f_bsize * fsData.f_bfree;
    // cout << "freeSpace: " << freeSpace << "fsData: " << fsData.f_bsize << "\t" << fsData.f_bfree     << endl;
    return (freeSpace > size);
}


void writeFile(string path, vector<BYTE>data){
    // cout << "path: "<< path<<endl;
    string strData(data.begin(), data.end());
    ofstream file(path);
    if (file.is_open()){
        file << strData;
        file.close();
    }
    else cout << "Unable to open file";
}
