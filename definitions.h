#ifndef __DEFINITIONS__
#define __DEFINITIONS__

#include <iostream>
#include <string.h>
#include <vector>

using namespace std;

#define MAXSIZE 64 //number maximum of bytes in data field
#define MINSIZE 60 //number minimum of bytes in data field

#define BEGIN 0x7E //begin delimiter value

#define DEVICE "lo"
#define NACK 0
#define ACK 1
#define CD 3
#define LS 4
#define PUT 5
#define GET 6
#define OK 8
#define SIZE 9
#define OUTPUT 10
#define DATA 13
#define ERROR 14
#define ENDTX 15

#define BYTE unsigned char

typedef struct{
	int begin   : 8,
        size    : 6,
        sequence: 6,
        type    : 4,
        parity  : 8;
}i_Control;

typedef struct{
	char begin,size,seqType,parity;
}c_Control;

typedef union {
	c_Control c_ctrl;
    i_Control i_ctrl;
}Message;


#endif