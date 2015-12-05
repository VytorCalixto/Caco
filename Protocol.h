#ifndef __PROTOCOL__
#define __PROTOCOL__
#include "definitions.h"
#include "Message.h"

class Protocol{

private:
    vector<Message> messages;
    int timeout;
public:
    bool sendMessages(int socket, int window);
    vector<Message> getMessages();
    void setMessages(vector<Message> messages);
    vector<BYTE> getData();
    void setData(vector<BYTE> data, int type);
    string getDataAsString();
    int recvMessage(int sockt);
    void addMessage(Message msg);
    void transmit(int sockt, int type, int window);
    void receive(int sockt, int type, int window);

    Protocol();
};
#endif
