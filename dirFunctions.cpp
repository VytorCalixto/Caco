#include <iostream>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <vector>
#include <numeric> //accumulate
#include <stdio.h> //popen
#include <iterator>

using namespace std;

string concatStringVector(vector<string> vs){
    ostringstream oss;
    copy(vs.begin(), vs.end(), ostream_iterator<string>(oss, " "));
    return oss.str();
}

void cd(string path){
    if(chdir(path.c_str()) != 0){
        cout << "Error: could not change directory." << endl;
        cerr << strerror(errno) << endl;
    }
}

string ls(vector<string> args){
    string output, command = concatStringVector(args);
    FILE *lsOut = popen(command.c_str(), "r");
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