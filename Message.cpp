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
    int size = data.size();
    //TODO: quebrar em partes aqui? ou tem que quebrar no protocolo?
    header.i_ctrl.size = size;
    header.i_ctrl.sequence = 1;
    char *fill;
    if(size < 63) {
        fill = (char*) malloc((63-size)*sizeof(char));
        memset(fill, 0, (63-size)*sizeof(char));
    }
    string d;
    d += header.c_ctrl.begin;
    d += header.c_ctrl.sizeSeq;
    d += header.c_ctrl.seqType;
    d += s.c_str();
    d += fill;
    d += header.c_ctrl.parity;
    return d;
}

Message::Message() {
    this->header.c_ctrl.begin = BEGIN;
}
