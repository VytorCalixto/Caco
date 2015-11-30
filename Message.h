#ifndef __MESSAGE__
#define __MESSAGE__
#include "definitions.h"

class Message {

public:
    Header header;
    vector<BYTE> data;
    BYTE calcParity();
    bool checkParity();
    string getSendData();

    Message();
};
#endif
