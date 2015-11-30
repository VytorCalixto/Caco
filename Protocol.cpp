#include <sys/socket.h>
#include "Protocol.h"
#include "definitions.h"

vector<Message> Protocol::getMessages(){
    return messages;
}

void Protocol::setMessages(vector<Message> messages){
    this->messages = messages;
}

bool Protocol::sendMessages(int socket, int window) {
    for(int i=0; i < messages.size(); ++i) {
        string data = messages[i].getSendData();
        send(socket, data.c_str(), messages[i].header.i_ctrl.size, 0);
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
    for (i=0; i <= data.size()-MAXSIZE; i+=MAXSIZE){
        Message msg;
        msg.header.i_ctrl.begin = BEGIN;
        msg.header.i_ctrl.size = MAXSIZE;
        msg.header.i_ctrl.sequence = messages.size()%(MAXSIZE+1);
        msg.header.i_ctrl.type = type;
        first = data.begin()+i;
        last = data.begin()+i+MAXSIZE+1;
        vector<BYTE> subvector(first, last);
        msg.data = subvector;
        //TODO: Set parity
        messages.push_back(msg);
    }
    if(i < data.size()){
        Message msg;
        msg.header.i_ctrl.begin = BEGIN;
        msg.header.i_ctrl.sequence = messages.size()%(MAXSIZE+1);
        msg.header.i_ctrl.type = type;
        int size = data.size()-i;
        first = data.begin()+i;
        last = data.begin()+size+1;
        vector<BYTE> subvector(first, last);
        msg.data = subvector;
        if(size < MINSIZE){
            size = MINSIZE;
            msg.data.insert(msg.data.begin(), MINSIZE-size, 0x0);
        }
        msg.header.i_ctrl.size = size;
        //TODO: Set parity
        messages.push_back(msg);
    }
}

int Protocol::recvMessage(int sockt){
    BYTE dataRec[MAXSIZE+4];
    recv(sockt, dataRec, MAXSIZE, 0);
    cout << dataRec[0] << dataRec[1] << dataRec[2] << "|\t";
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
    // TODO: Check parity
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
