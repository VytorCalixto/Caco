#include "Protocol.h"
#include "definitions.h"

Message Protocol::getMessage(){
    return message;
}

void Protocol::setMessage(Message message){
    this->message = message;
}

int Protocol::readMessage(int sockt){
    unsigned char* dataRec = recv(sockt, data, MAXSIZE, 0);
    Message msg;
    msg.c_ctrl.begin = data[0];
    if(msg.i_ctrl.begin != BEGIN){
        return 0;
    }
    msg.c_ctrl.size = data[1];
    msg.c_ctrl.seqType = data[2];
    msg.c_ctrl.parity = data[3+msg.i_ctrl.size];
    // TODO: Check parity
    if(msg.i_ctrl.type == FIM){
        this->message = msg;
        return 1;
    }

    unsigned char data[msg.i_ctrl.size];
    strncat(data,dataRec+3,msg.i_ctrl.size);
    this->data.insert(this->data.end(), data, data+msg.i_ctrl.size);
    this->message = msg;
    return -1;
}

Protocol::Protocol(){
    message.i_ctrl.begin = BEGIN;
}