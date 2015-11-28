#include "definitions.h"
#include "Protocol.h"
#include "dirFunctions.h"
#include "ConexaoRawSocket.c"

int main(){
    int sockt = ConexaoRawSocket(DEVICE);
    Protocol protocol;
    while(true){
        int status = protocol.recvMessage(sockt);
        if(status > 0){
            if(status == ENDTX){
                protocol = Protocol();
                //TODO: send ACK
            }else if(status == CD){
                cd(protocol.getDataAsString());
            }else if(status == LS){
                string output = ls(protocol.getDataAsString());
                //TODO: send output back
            }else if(status == PUT){
                //TODO
            }else if(status == GET){
                //TODO
            }
        }
    }
    return 0;
}