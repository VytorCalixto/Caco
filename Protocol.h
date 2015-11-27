#ifndef __PROTOCOL__
#define __PROTOCOL__
#include "definitions.h"

class Protocol{

private:
    Message message;
    vector<BYTE> data;
    int timeout;
public:

    Message getMessage();
    void setMessage(Message message);
    vector<BYTE> getData();
    void setData(vector<BYTE> data);
    string getDataAsString();
    int readMessage(int sockt);

    Protocol();
};
#endif