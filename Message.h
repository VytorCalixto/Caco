#ifndef __MESSAGE__
#define __MESSAGE__
#include "definitions.h"

class Message {

public:
    bitset<BEGIN_S> begin;
    bitset<SIZE_S> size;
    bitset<SEQUENCE_S> sequence;
    bitset<TYPE_S> type;
    bitset<PARITY_S> parity;
    vector<BYTE> data;

    BYTE calcParity();
    bool checkParity();
    void setBitFields(BYTE begin, BYTE sizeSeq, BYTE seqType, BYTE parity);
    vector<BYTE> getBitFieldsAsBytes();
    vector<BYTE> getMessage();
    char* getMessageAsCharPointer();
    int getMessageSize();
    int dataToInt();

    Message();
};
ostream& operator<<(ostream& os, const Message& msg);
#endif
