#include "Message.h"
#include <bitset>
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

vector<int> Message::getBitFieldsAsInt(){
    vector<int> bits;
    // bitset<8> begin(messages[i].header.i_ctrl.begin);
    // bitset<6> size(messages[i].header.i_ctrl.size);
    // bitset<6> sequence(messages[i].header.i_ctrl.sequence);
    // bitset<4> type(messages[i].header.i_ctrl.type);
    // bitset<8> parity(messages[i].header.i_ctrl.parity);
    bits.push_back((int)bitset<8>(header.i_ctrl.begin).to_ulong());
    bits.push_back((int)bitset<6>(header.i_ctrl.size).to_ulong());
    bits.push_back((int)bitset<6>(header.i_ctrl.sequence).to_ulong());
    bits.push_back((int)bitset<4>(header.i_ctrl.type).to_ulong());
    bits.push_back((int)bitset<8>(header.i_ctrl.parity).to_ulong());
    return bits;

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
    this->header.i_ctrl.begin = BEGIN;
}
