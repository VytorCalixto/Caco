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
        cout << "message: " << messages[i] << endl;
        vector<BYTE> message = messages[i].getMessage();
        char* msg = reinterpret_cast<char*> (message.data());
        cout << "char* msg: ";
        for(int j=0;j<message.size();++j){
            cout << bitset<8>(msg[j]);
        }
        cout <<endl;
        send(socket, msg, messages[i].getMessageSize(), 0);
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
        msg.size = bitset<SIZE_S>(MAXSIZE);
        msg.sequence = bitset<SEQUENCE_S>(messages.size()%(MAXSIZE+1));
        msg.type = bitset<TYPE_S>(type);
        first = data.begin()+i;
        last = data.begin()+i+MAXSIZE+1;
        vector<BYTE> subvector(first, last);
        msg.data = subvector;
        msg.calcParity();
        messages.push_back(msg);
    }
    if(i < data.size()){
        Message msg = Message();
        msg.sequence = bitset<SEQUENCE_S>(messages.size()%(MAXSIZE+1));
        msg.type = bitset<TYPE_S>(type);
        int size = ((int)data.size())-i;
        first = data.begin()+i;
        last = data.begin()+size+1;
        vector<BYTE> subvector(first, last);
        msg.data = subvector;
        if(size < MINSIZE){
            BYTE zero = 0x00;
            msg.data.insert(msg.data.begin(), MINSIZE-size, zero);
            size = MINSIZE;
        }
        msg.size = bitset<SIZE_S>(size);
        msg.calcParity();
        messages.push_back(msg);
    }
}

int Protocol::recvMessage(int sockt){
    BYTE dataRec[MAXSIZE+4];
    int r = recv(sockt, dataRec, MAXSIZE+4, 0);
    cout << bitset<8>(dataRec[0]) << bitset<8>(dataRec[1]) << bitset<8>(dataRec[2]) << "|\t";
    cout << "recv response: " << r << endl;
    if(dataRec[0] != BEGIN){
        return NOISE;
    }
    Message msg = Message();
    int size = (int)(dataRec[1]>>2);
    msg.setBitFields(dataRec[0], dataRec[1], dataRec[2], dataRec[size+3]);
    cout << "Sequence:" << msg.sequence.to_ulong() << endl;
    // FIXME: Erro na primeira mensagem de sequencialização
    // if(msg.sequence.to_ulong() != ((messages.back().sequence.to_ulong()+1)%(MAXSIZE+1))){
    //     return SEQ_MISS;
    // }
    if(!msg.checkParity()){
        return INCONSISTENT;
    }
    if(msg.type.to_ulong() == ENDTX){
        return ENDTX;
    }

    BYTE msgData[size];
    memcpy(msgData,dataRec+3,size);
    msg.data.insert(msg.data.end(), msgData, msgData+size);
    messages.push_back(msg);
    cout << "Tipo:" << (int)msg.type.to_ulong() << endl;
    return (int)msg.type.to_ulong();
}

void Protocol::transmit(int sockt, int type, int window){
    int status;
    for(int i=0; i < messages.size(); ++i){
        //TODO: send part of output back
        status = recvMessage(sockt);
        if(!status){
            //TODO: send part of output back again
        }else if(status == 1){
            //TODO: send another part of output back
        }else{
            //TODO: send NACK back
        }
    }
}

void Protocol::receive(int sockt, int type, int window){
    int status;
    do{
        status = recvMessage(sockt);
        if(status == type){
            //TODO: send ACK back
        }else{
            //TODO: send NACK back
        }
    }while(status != ENDTX);
}

void Protocol::addMessage(Message msg) {
    messages.push_back(msg);
}

Protocol::Protocol(){
}
