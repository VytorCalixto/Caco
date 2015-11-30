#ifndef __DEFINITIONS__
#define __DEFINITIONS__

#include <iostream>
#include <string.h>
#include <vector>
#include <cstdlib>

using namespace std;

//Maximum and minimum of bytes in data field
#define MAXSIZE 63
#define MINSIZE 60

//Delimiter indicating beginning of a message
#define BEGIN 0x7E
//Socket device
#define DEVICE "lo"
//Message types
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

//Windows
#define WAIT_STOP 1
#define SLIDING 3

//Values of data for ERROR messages
#define DIR_ERR '0'
#define PERM_ERR '1'
#define SPACE_ERR '2'

//recvMessage errors
#define NOISE -1
#define SEQ_MISS -2
#define INCONSISTENT -3

//Data types
#define BYTE unsigned char

typedef struct{
	int begin   : 8,
        size    : 6,
        sequence: 6,
        type    : 4,
        parity  : 8;
}i_Control;

typedef struct{
	char begin,sizeSeq,seqType,parity;
}c_Control;

typedef union {
	c_Control c_ctrl;
    i_Control i_ctrl;
}Header;

#endif
