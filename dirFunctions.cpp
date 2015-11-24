#include <iostream>
#include <errno.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

using namespace std;

void cd(string path){
    if(chdir(path.c_str()) != 0){
        cout << "Error: could not change directory." << endl;
        cerr << strerror(errno) << endl;
    }
}

void ls(string path){
    //TODO: #1
    struct dirent *entry;
    DIR *dir = opendir(path.c_str());
    if(dir != NULL){
        while((entry=readdir(dir)) != NULL){
           cout << entry->d_name << endl;
        }
        closedir(dir);
    }else{
        cout<<"Error: could not open directory."<<endl;
    }
}