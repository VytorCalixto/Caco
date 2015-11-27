#include <iostream>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <vector>
#include <numeric> //accumulate
#include <stdio.h> //popen
#include <iterator>

using namespace std;

void cd(string path){
    if(chdir(path.c_str()) != 0){
        cout << "Error: could not change directory." << endl;
        cerr << strerror(errno) << endl;
    }
}

string ls(string args){
    string output;
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
    //TODO: #1
    // struct dirent *entry;
    // DIR *dir = opendir(path.c_str());
    // if(dir != NULL){
    //     while((entry=readdir(dir)) != NULL){
    //        cout << entry->d_name << endl;
    //     }
    //     closedir(dir);
    // }else{
    //     cout<<"Error: could not open directory."<<endl;
    // }
}