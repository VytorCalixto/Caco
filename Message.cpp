#include "Message.h"
#include "definitions.h"

BYTE Message::calcParity() {
    BYTE parity = 0x00;
    vector<BYTE> m = getMessage();
    //Begin and parity bytes are not taken in account in parity
    for(int i=1; i < m.size()-1; ++i) {
        parity = parity^m[i];
    }
    this->parity = parity;
    return parity;
}

bool Message::checkParity() {
    BYTE parity = this->calcParity();
    return (parity == this->parity.to_ulong());
}

void Message::setBitFields(BYTE begin, BYTE sizeSeq, BYTE seqType, BYTE parity){
    //if the size of val passed in the bitset constructor is greater than the bitset size,
    // only the least significant bits of val are taken into consideration.
    this->begin = bitset<BEGIN_S>(begin);
    this->size = bitset<SIZE_S>(sizeSeq >> 2);
    this->sequence = bitset<SEQUENCE_S>((sizeSeq << 4) | (seqType >> 4));
    this->type = bitset<TYPE_S>(seqType);
    this->parity = bitset<PARITY_S>(parity);
}

vector<BYTE> Message::getBitFieldsAsBytes(){
    vector<BYTE> bitFields;
    bitFields.push_back((BYTE)begin.to_ulong());
    bitFields.push_back((BYTE)size.to_ulong());
    bitFields.push_back((BYTE)sequence.to_ulong());
    bitFields.push_back((BYTE)type.to_ulong());
    bitFields.push_back((BYTE)parity.to_ulong());
    return bitFields;
}

vector<BYTE> Message::getMessage(){
    vector<BYTE> msg = getBitFieldsAsBytes();
    msg.insert(msg.end()-2, data.begin(), data.end());
    return msg;
}

char* Message::getMessageAsCharPointer(){
    return reinterpret_cast<char*> (getMessage().data());
}

int Message::getMessageSize(){
    return size.to_ulong()+4;
}

// string Message::getSendData() {
//     calcParity();
//     string s(data.begin(), data.end());
//     int size = data.size();
//     //TODO: quebrar em partes aqui? ou tem que quebrar no protocolo?
//     header.i_ctrl.size = size;
//     header.i_ctrl.sequence = 1;
//     char *fill;
//     if(size < 63) {
//         fill = (char*) malloc((63-size)*sizeof(char));
//         memset(fill, 0, (63-size)*sizeof(char));
//     }
//     string d;
//     d += header.c_ctrl.begin;
//     d += header.c_ctrl.sizeSeq;
//     d += header.c_ctrl.seqType;
//     d += s.c_str();
//     d += fill;
//     d += header.c_ctrl.parity;
//     return d;
// }

ostream& operator<<(ostream& os, const Message& msg){
    os << '|' << msg.begin << '|' << msg.size << '|' << msg.sequence << '|' << msg.type << '|';
    for(int i=0; i<msg.data.size(); ++i){
        os << bitset<8>(msg.data[i]);
    }
    os << '|' << msg.parity << '|';
    return os;
}

Message::Message() {
    this->begin = bitset<BEGIN_S>(BEGIN);
}
