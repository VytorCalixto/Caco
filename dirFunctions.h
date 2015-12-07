#ifndef __DIRFUNCTIONS__
#define __DIRFUNCTIONS__
#include "dirFunctions.cpp"
#include <string.h>

using namespace std;

void cd(string path);

string ls(string path);

bool fexists(string path);

int filesize(string path);

#endif
