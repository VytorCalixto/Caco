#include "definitions.h"
#include "Protocol.h"
#include "dirFunctions.h"
#include "ConexaoRawSocket.c"

int main(){
    int sockt = ConexaoRawSocket(DEVICE);
    Protocol protocol;
    cout << "Você está rodando o servidor Caco\n";
    while(true){
        int status = protocol.recvMessage(sockt);
        cout << "status: " << status << endl;
        // cout << protocol.getDataAsString() << endl;
        if(status > 0){
            if(status == ENDTX){
                protocol = Protocol();
                //TODO: send ACK
            }else if(status == CD){
                cout << "Recebeu CD\n";
                cd(protocol.getDataAsString());
            }else if(status == LS){
                cout << protocol.getDataAsString() << endl;
                string output = ls(protocol.getDataAsString());
                cout << "LS:" << output << endl;
                protocol.setData(vector<BYTE>(output.begin(), output.end()), OUTPUT);
                protocol.sendMessages(sockt);
            }else if(status == PUT){
                //TODO
            }else if(status == GET){
                //TODO
            }
        }
    }
    return 0;
}
