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
    // cout << "Input:\t" << bitset<8>(sizeSeq) << "\t" << bitset<8>(seqType);
    // cout << "Set bitFields:\t";
    this->begin = bitset<BEGIN_S>(begin);
    this->size = bitset<SIZE_S>(sizeSeq >> 2);
    // cout << size << "\t";
    this->sequence = bitset<SEQUENCE_S>((sizeSeq << 4) | (seqType >> 4));
    // cout << sequence << "\t";
    this->type = bitset<TYPE_S>(seqType);
    // cout << type << endl;
    this->parity = bitset<PARITY_S>(parity);
}

vector<BYTE> Message::getBitFieldsAsBytes(){
    vector<BYTE> bitFields;
    bitFields.push_back((BYTE)begin.to_ulong());
    // bitFields.push_back((BYTE)size.to_ulong());
    // bitFields.push_back((BYTE)sequence.to_ulong());
    // bitFields.push_back((BYTE)type.to_ulong());
    string str = size.to_string();
    str += sequence.to_string();
    str += type.to_string();
    // cout << "str:" << str << "\t" << (BYTE)bitset<8>(str.substr(8,8)).to_ulong() << endl;
    bitFields.push_back((BYTE)bitset<8>(str.substr(0,8)).to_ulong());
    bitFields.push_back((BYTE)bitset<8>(str.substr(8,8)).to_ulong());
    bitFields.push_back((BYTE)parity.to_ulong());
    return bitFields;
}

vector<BYTE> Message::getMessage(){
    vector<BYTE> msg = getBitFieldsAsBytes();
    msg.insert(msg.end()-1, data.begin(), data.end());
    // cout << "getMessage:\n";
    // for(int i=0; i < msg.size(); ++i) {
    //     cout << bitset<8>(msg[i]);
    // }
    // cout<<endl;
    return msg;
}

char* Message::getMessageAsCharPointer(){
    return reinterpret_cast<char*> (getMessage().data());
}

int Message::getMessageSize(){
    return size.to_ulong()+4;
}

int Message::dataToInt() {
    string str(data.begin(), data.end());
    return stoi(str);
}

string Message::getDataAsString() {
    string str(data.begin(), data.end());
    return str;
}

ostream& operator<<(ostream& os, const Message& msg){
    os << '|' << msg.begin << '|' << msg.size << '|' << msg.sequence << '|' << msg.type << '|';
    // os << msg.begin << msg.size << msg.sequence << msg.type;
    for(int i=0; i<msg.data.size(); ++i){
        os << bitset<8>(msg.data[i]);
    }
    os << '|' << msg.parity << '|';
    // os << msg.parity;
    return os;
}

Message::Message() {
    this->begin = bitset<BEGIN_S>(BEGIN);
}
