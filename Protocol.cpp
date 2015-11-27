#include <sys/socket.h>
#include "Protocol.h"
#include "definitions.h"

Message Protocol::getMessage(){
    return message;
}

void Protocol::setMessage(Message message){
    this->message = message;
}

int Protocol::readMessage(int sockt){
    unsigned char dataRec[MAXSIZE+4];
    int ret = recv(sockt, dataRec, MAXSIZE, 0);
    Message msg;
    msg.c_ctrl.begin = dataRec[0];
    if(msg.i_ctrl.begin != BEGIN){
        return 0;
    }
    msg.c_ctrl.size = dataRec[1];
    msg.c_ctrl.seqType = dataRec[2];
    msg.c_ctrl.parity = dataRec[3+msg.i_ctrl.size];
    // TODO: Check parity
    if(msg.i_ctrl.type == FIM){
        this->message = msg;
        return 1;
    }

    unsigned char msgData[msg.i_ctrl.size];
    memcpy(msgData,dataRec+3,msg.i_ctrl.size);
    this->data.insert(this->data.end(), msgData, msgData+msg.i_ctrl.size);
    this->message = msg;
    return -1;
}

Protocol::Protocol(){
    message.i_ctrl.begin = BEGIN;
}