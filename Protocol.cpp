#include <sys/socket.h>
#include <sys/time.h>
#include "Protocol.h"
#include "definitions.h"

typedef struct{
    int index;
    bool sent;
}FrameItem;

double timestamp(void) {
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return ((double)(tp.tv_sec + tp.tv_usec/1000000.0));
}

vector<Message> Protocol::getMessages(){
    return messages;
}

void Protocol::setMessages(vector<Message> messages){
    this->messages = messages;
}

bool Protocol::sendMessages(int socket) {
    bool success = true;
    for(int i=0; i < messages.size(); ++i) {
        int status = sendMessage(socket, i);
        if(status < 0) {
            success = false;
        }
    }
    return success;
}

bool Protocol::sendMessage(int socket, int index) {
    // cout << "message sent: " << messages[index] << endl;
    vector<BYTE> message = messages[index].getMessage();
    unsigned char* msg = reinterpret_cast<unsigned char*> (message.data());
    int status = send(socket, msg, messages[index].getMessageSize(), 0);
    return (status >= 0);
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

int Protocol::setData(vector<BYTE> data, int type){
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
        last = data.begin()+i+size+1;
        vector<BYTE> subvector(first, last);
        msg.data = subvector;
        msg.size = bitset<SIZE_S>(size);
        msg.calcParity();
        if(size < MINSIZE){
            BYTE zero = 0x00;
            msg.data.insert(msg.data.end(), MINSIZE-size, zero);
        }
        msg.size = bitset<SIZE_S>(size);
        msg.calcParity();
        messages.push_back(msg);
    }
    return messages.size();
}

int Protocol::recvMessage(int sockt){
    BYTE dataRec[MAXSIZE+4];
    int r = recv(sockt, dataRec, MAXSIZE+4, 0);
    // cout << "begin: "<< bitset<8>(dataRec[0]) <<endl;
    // cout << "recv response: " << r << endl;
    if(dataRec[0] != BEGIN){
        return NOISE;
    }
    Message msg = Message();
    int size = (int)(dataRec[1]>>2);
    // cout << "Tamanho:" << size << "\t";
    int dataSize = size < MINSIZE ? MINSIZE : size;
    msg.setBitFields(dataRec[0], dataRec[1], dataRec[2], dataRec[dataSize+3]);
    // cout << "Sequence:" << msg.sequence.to_ulong() << "\t";
    BYTE msgData[size];
    memcpy(msgData,dataRec+3,size);
    msg.data.insert(msg.data.end(), msgData, msgData+size);

    messages.push_back(msg);
    // cout << "Tipo:" << (int)msg.type.to_ulong() << endl;

    // cout <<"message received: "<< msg<<endl;
    if(!msg.checkParity()){
        return INCONSISTENT;
    }
    return (int)msg.type.to_ulong();
}

void Protocol::transmit(int sockt, int window){
    int status;
    vector<FrameItem> frame;
    int lastFramed = -1;
    setData(vector<BYTE>(1, 0), ENDTX);
    int messagesLeft = messages.size();
    Protocol response;
    double t0 = 0.0;
    bool timeout = false;
    while(messagesLeft > 0){
        cout << "Restantes: " << messagesLeft << endl;
        for(int j=0; j < window; ++j) {
            if(frame.size() < window && frame.size() < messagesLeft) {
                // cout << "frame size: " << frame.size() << endl;
                FrameItem fi = {.index = ++lastFramed, .sent=false};
                // cout << "lastFramed: " << fi.index << endl;
                frame.push_back(fi);
            }
            if(!frame[j].sent || timeout){
                sendMessage(sockt, frame[j].index);
                frame[j].sent = true;
                t0 = timestamp();
            }
        }
        // TODO: timeout
        status = response.recvMessage(sockt);
        // cout << "transmit status:" << status << endl;
        if(status == NACK){
            // printf("nstoi %i\n", response.getMessages().back().getDataAsString()[0]);
            int nackIndex = response.getMessages().back().getDataAsString()[0];
            for(int j=0; j < window;) {
                if(frame[j].index < nackIndex) {
                    frame.erase(frame.begin() + j);
                    --messagesLeft;
                } else if(frame[j].index == nackIndex) {
                    frame[j].sent = false;
                } else ++j;
            }
            response.reset();
            t0 = timestamp();

        }else if(status == ACK){
            // printf("astoi %i\n", response.getMessages().back().getDataAsString()[0]);
            int ackIndex = response.getMessages().back().getDataAsString()[0];
            for(int j=0; j < frame.size();) {
                // cout << "ackIndex: " << ackIndex << "frame[j].index: " << frame[j].index << endl;
                if(ackIndex == 0) {
                    if((frame[j].index % (MAXSIZE+1)) == 62 || (frame[j].index % (MAXSIZE+1)) == 63 || (frame[j].index % (MAXSIZE+1)) == 0) {
                        frame.erase(frame.begin() + j);
                        --messagesLeft;
                    } else ++j;
                } else if((frame[j].index % (MAXSIZE+1)) <= ackIndex) {
                    frame.erase(frame.begin() + j);
                    --messagesLeft;
                } else ++j;
            }
            response.reset();
            t0 = timestamp();
        } else {
            //TODO: treat error
        }
        timeout = ((timestamp() - t0) > 2.0);
        if(timeout) cout << "TIMEOUT!\n";
    }
    reset();
}

int Protocol::receive(int sockt, int type, int window, bool dataEndable){
    int status, nextSequence = 0;
    vector<int> frame;
    Protocol response;
    bool shouldSend = false;
    int end = ((dataEndable)? ENDTX : type);
    do{
        if(shouldSend){
            response.sendMessages(sockt);
            response.reset();
            shouldSend = false;
        }
        status = recvMessage(sockt);
        // cout << "receive status:" << status << endl;
        // cout << "sequence: "<<messages.back().sequence.to_ulong()<<" next: "<<nextSequence<<endl;
        if(status == NOISE){
            continue;
        } else if(status == type) {
            if(!messages.empty() && (messages.back().sequence.to_ulong() != nextSequence)){
                messages.pop_back();
                response.reset();
                vector<BYTE> val(1,(BYTE)nextSequence);
                response.setData(val, NACK);
                shouldSend = true;
            } else {
                if(window == WAIT_STOP || ((nextSequence % SLIDING) == 0)) {
                    response.reset();
                    vector<BYTE> val(1,(BYTE)messages.back().sequence.to_ulong());
                    response.setData(val, ACK);
                    shouldSend = true;
                }
                nextSequence = (messages.back().sequence.to_ulong()+1)%(MAXSIZE+1);
            }
        } else if(status == ERROR) {
            cout << "ERROR: " << messages.back().getDataAsString() << endl;
            return -1;
        }
    }while(status != end);
    if(dataEndable){
        response.reset();
        vector<BYTE> val(1,(BYTE)messages.back().sequence.to_ulong());
        response.setData(val, ACK);
        response.sendMessages(sockt);
        messages.pop_back();
    }
    return 0;
}

void Protocol::addMessage(Message msg) {
    messages.push_back(msg);
}

void Protocol::reset(){
    messages.clear();
}

Protocol::Protocol(){
}
