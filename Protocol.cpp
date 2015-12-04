#include <sys/socket.h>
#include <bitset>
#include "Protocol.h"
#include "definitions.h"

vector<Message> Protocol::getMessages(){
    return messages;
}

void Protocol::setMessages(vector<Message> messages){
    this->messages = messages;
}

bool Protocol::sendMessages(int socket, int window) {
    //FIXME: seqType char is wrong, could be because of how sequence is being set
    for(int i=0; i < messages.size(); ++i) {
        vector<BYTE> msg;
        msg.push_back(messages[i].header.c_ctrl.begin);
        cout << "vector begin: "<< bitset<8>(msg[0]) << endl;
        cout << "sizeSeq: " << bitset<8>(messages[i].header.c_ctrl.sizeSeq) << endl;
        cout << "seqType: " << bitset<8>(messages[i].header.c_ctrl.seqType) << endl;
        msg.push_back(messages[i].header.c_ctrl.sizeSeq);
        cout << "vector sizeSeq: "<< bitset<8>(msg[1]) << endl;
        msg.push_back(messages[i].header.c_ctrl.seqType);
        msg.insert(msg.end(), messages[i].data.begin(), messages[i].data.end());
        msg.push_back(messages[i].header.c_ctrl.parity);
        char* data = reinterpret_cast<char*> (msg.data());
        cout << "msg size: " << bitset<6>(messages[i].header.i_ctrl.size) << endl;
        cout << "pointer begin: "<< bitset<8>(data[0]) << "|\t";
        cout << "msg isize: " << messages[i].getBitFieldsAsInt()[1] << endl;
        send(socket, data, messages[i].getBitFieldsAsInt()[1]+4, 0);
    }
    return true;
}

vector<BYTE> Protocol::getData(){
    vector<BYTE> fullData;
    for (int i=0; i < messages.size(); ++i){
        fullData.insert(fullData.end(), messages[i].data.begin(), messages[i].data.end());
    }
    return fullData;
}

string Protocol::getDataAsString(){
    vector<BYTE> fullData = getData();
    string str(fullData.begin(), fullData.end());
    return str;
}

void Protocol::setData(vector<BYTE> data, int type){
    vector<BYTE>::const_iterator first, last;
    int i;
    for (i=0; i <= ((int)data.size())-MAXSIZE; i+=MAXSIZE){
        Message msg = Message();
        msg.header.i_ctrl.size = MAXSIZE;
        msg.header.i_ctrl.sequence = messages.size()%(MAXSIZE+1);
        msg.header.i_ctrl.type = type;
        first = data.begin()+i;
        last = data.begin()+i+MAXSIZE+1;
        vector<BYTE> subvector(first, last);
        msg.data = subvector;
        msg.calcParity();
        messages.push_back(msg);
    }
    if(i < data.size()){
        cout << "entrou if"<< endl;
        Message msg = Message();
        msg.header.i_ctrl.sequence = messages.size()%(MAXSIZE+1);
        msg.header.i_ctrl.type = type;
        int size = ((int)data.size())-i;
        cout << "real size " << size<<endl;
        first = data.begin()+i;
        last = data.begin()+size+1;
        vector<BYTE> subvector(first, last);
        msg.data = subvector;
        if(size < MINSIZE){
            size = MINSIZE;
            msg.data.insert(msg.data.begin(), MINSIZE-size, 0x0);
        }
        cout << "modified size " << size<<endl;
        msg.header.i_ctrl.size = size;
        msg.calcParity();
        messages.push_back(msg);
    }
}

int Protocol::recvMessage(int sockt){
    BYTE dataRec[MAXSIZE+4];
    int r = recv(sockt, dataRec, MAXSIZE+4, 0);
    cout << "recv response: " << r << endl;
    cout << bitset<8>(dataRec[0]) << bitset<8>(dataRec[1]) << bitset<8>(dataRec[2]) << "|\t";
    Message msg;
    msg.header.c_ctrl.begin = dataRec[0];
    if(msg.header.i_ctrl.begin != BEGIN){
        return NOISE;
    }
    msg.header.c_ctrl.sizeSeq = dataRec[1];
    msg.header.c_ctrl.seqType = dataRec[2];
    if(msg.header.i_ctrl.sequence != ((messages.back().header.i_ctrl.sequence+1)%(MAXSIZE+1))){
        return SEQ_MISS;
    }
    msg.header.c_ctrl.parity = dataRec[3+msg.header.i_ctrl.size];
    if(!msg.checkParity()){
        return INCONSISTENT;
    }
    if(msg.header.i_ctrl.type == ENDTX){
        return ENDTX;
    }

    BYTE msgData[msg.header.i_ctrl.size];
    memcpy(msgData,dataRec+3,msg.header.i_ctrl.size);
    msg.data.insert(msg.data.end(), msgData, msgData+msg.header.i_ctrl.size);
    messages.push_back(msg);
    return msg.header.i_ctrl.type;
}

// void Protocol::recvWholeData(int sockt, int window){
//     int status;
//     while((status=this->recvMessage(sockt)) != ENDTX){
//         if(status > 0){
//             if(messages.back().header.i_ctrl.sequence % window == 0){
//                 //TODO: send ACK
//             }
//         }
//     }
// }

void Protocol::addMessage(Message msg) {
    messages.push_back(msg);
}

Protocol::Protocol(){
}
