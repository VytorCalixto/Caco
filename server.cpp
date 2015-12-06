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
        if(status > 0){
            if(status == ENDTX){
                // protocol = Protocol();
                //TODO: send ACK
            }else if(status == CD){
                cout << "Recebeu CD\n";
                cd(receiveProtocol.getDataAsString());
            }else if(status == LS){
                string output = ls(receiveProtocol.getDataAsString());
                cout << "LS: " << output << endl;
                // receiveProtocol = Protocol();
                receiveProtocol.setData(vector<BYTE>(output.begin(), output.end()), OUTPUT);
                receiveProtocol.sendMessages(sockt);
                // sendProtocol = Protocol();
            }else if(status == PUT){
                //TODO
            }else if(status == GET){
                //TODO
            }
        }
    }
    return 0;
}
