#include "Protocol.h"
#include "definitions.h"

Message Protocol::getMessage(){
    return message;
}

void Protocol::setMessage(Message message){
    this->message = message;
}

bool Protocol::send(int socket) {
    return true;
}

char Protocol::calcParity() {
    char parity = 0x00;
    char *m = this->message.c_ctrl.begin + this->message.c_ctrl.size + this->message.c_ctrl.seqType + (&data[0]);
    for(int i=0; i < strlen(m); ++i) {
        parity = parity^m[i];
    }
    this->message.c_ctrl.parity = parity;
    return parity;
}

bool Protocol::checkParity() {
    char parity = this->calcParity();
    return (parity == this->message.c_ctrl.parity);
}

Protocol::Protocol(){
    this->message.c_ctrl.begin = 0x7E;
}
