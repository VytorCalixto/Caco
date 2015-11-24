#ifndef __DEFINITIONS__
#define __DEFINITIONS__

#include <iostream>
#include <string.h>
#include <vector>

using namespace std;

typedef struct {
    int begin   : 8,
        size    : 6,
        sequence: 6,
        type    : 4,
        parity  : 8;
}Message;

#endif