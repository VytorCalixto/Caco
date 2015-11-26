#ifndef __PROTOCOL__
#define __PROTOCOL__
#include "definitions.h"

class Protocol{

private:
    Message message;
    vector<unsigned char> data;
    int timeout;
public:

    Message getMessage();
    void setMessage(Message message);
    int readMessage(int socket);

    Protocol();
};
#endif