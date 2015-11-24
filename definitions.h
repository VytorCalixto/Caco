#ifndef __DEFINITIONS__
#define __DEFINITIONS__

#include <iostream>
#include <string.h>
#include <vector>

using namespace std;

#define DEVICE "lo"
#define NACK 0
#define ACK 1
#define CD 3
#define LS 4
#define PUT 5
#define GET 6
#define OK 8
#define TAM 9
#define TELA 10
#define ERRO 14
#define FIM 15

typedef struct {
    int begin   : 8,
        size    : 6,
        sequence: 6,
        type    : 4,
        parity  : 8;
}Message;

#endif