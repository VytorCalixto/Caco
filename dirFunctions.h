#ifndef __DIRFUNCTIONS__
#define __DIRFUNCTIONS__
#include "dirFunctions.cpp"
#include <string.h>

using namespace std;

void cd(string path);

string ls(string path);

bool hasEnoughSpace(int size);

string getWorkingPath();

void writeFile(string path, vector<BYTE>data);

#endif