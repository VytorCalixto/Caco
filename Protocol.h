#ifndef __PROTOCOL__
#define __PROTOCOL__
#include "definitions.h"

class Protocol{

private:
    Message message;
    vector<char> data;
public:

    Message getMessage();
    void setMessage(Message message);
    bool send(int socket);
    char calcParity();
    bool checkParity();

    Protocol();
};
#endif
