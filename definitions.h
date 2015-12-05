#ifndef __DEFINITIONS__
#define __DEFINITIONS__

#include <iostream>
#include <string.h>
#include <vector>
#include <bitset>
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
#define DIR_ERR '0' //Nonexistent Directory
#define PERM_ERR '1' //Permission denied
#define SPACE_ERR '2' //File size bigger than disk free space size

//recvMessage errors
#define NOISE -1
#define SEQ_MISS -2
#define INCONSISTENT -3

//Bit fields size
#define BEGIN_S 8
#define SIZE_S 6
#define SEQUENCE_S 6
#define TYPE_S 4
#define PARITY_S 8

//Data types
#define BYTE unsigned char


#endif
