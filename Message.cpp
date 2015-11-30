#include "Message.h"
#include "definitions.h"

BYTE Message::calcParity() {
    char parity = 0x00;
    char *m = this->header.c_ctrl.begin + this->header.c_ctrl.sizeSeq + this->header.c_ctrl.seqType + (char *)(&data[0]);
    for(int i=0; i < strlen(m); ++i) {
        parity = parity^m[i];
    }
    this->header.c_ctrl.parity = parity;
    return parity;
}

bool Message::checkParity() {
    BYTE parity = this->calcParity();
    return (parity == this->header.c_ctrl.parity);
}

string Message::getSendData() {
    calcParity();
    string s(data.begin(), data.end());
    string d = header.c_ctrl.begin + header.c_ctrl.sizeSeq + header.c_ctrl.seqType + s.c_str() + header.c_ctrl.parity;
    return d;
}

Message::Message() {
    this->header.c_ctrl.begin = BEGIN;
}
