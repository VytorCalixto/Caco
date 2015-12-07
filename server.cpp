#include "definitions.h"
#include "Protocol.h"
#include "dirFunctions.h"
#include "ConexaoRawSocket.c"

int main(){
    int sockt = ConexaoRawSocket(DEVICE);
    Protocol receiveProtocol, sendProtocol;
    cout << "Você está rodando o servidor Caco\n";
    while(true){
        int status = receiveProtocol.recvMessage(sockt);
        cout << "status: " << status << endl;
        // cout << protocol.getDataAsString() << endl;
        try{
            if(status == NOISE){
                continue;
            }
            if(status == CD){
                cout << "Recebeu CD\n";
                cout << "CD: " << receiveProtocol.getDataAsString() << endl;
                cd(receiveProtocol.getDataAsString());
                vector<BYTE> val(1,(BYTE)0);
                sendProtocol.setData(val, OK);
                sendProtocol.sendMessage(sockt,0);
            }else if(status == LS){
                cout << "protocol data: " << receiveProtocol.getDataAsString() << endl;
                cout << "message data: " << receiveProtocol.getMessages().back().getDataAsString() << endl;
                string output = ls(receiveProtocol.getMessages().back().getDataAsString());
                cout << "LS: " << output << endl;
                sendProtocol.setData(vector<BYTE>(output.begin(), output.end()), OUTPUT);
                sendProtocol.transmit(sockt, WAIT_STOP);
                cout << "finished transmit" << endl;
            }else if(status == PUT){
                //TODO
            }else if(status == GET){
                //TODO
            }
        }catch(char const* strException){
            cout << "Erro:" <<strException <<endl;
        }
        sendProtocol.reset();
        receiveProtocol.reset();
    }
    return 0;
}
