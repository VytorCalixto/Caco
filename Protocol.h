#ifndef __PROTOCOL__
#define __PROTOCOL__
#include "definitions.h"

class Protocol{

private:
    vector<Message> messages;
    int timeout;
public:

    vector<Message> getMessages();
    void setMessages(vector<Message> messages);
    vector<BYTE> getData();
    void setData(vector<BYTE> data, int type);
    string getDataAsString();
    int recvMessage(int sockt);

    Protocol();
};
#endif