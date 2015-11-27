#include "definitions.h"
#include "dirFunctions.h"
#include "ConexaoRawSocket.c"

int main(){
    int sockt = ConexaoRawSocket(DEVICE);
    Protocol protocol;
    while(true){
        int status = protocol.readMessage(sockt);
        if(status > 0){
            if(status == ENDTX){
                protocol = Protocol();
                if(protocol.getMessage().i_ctrl.type == DATA){
                    //TODO: send ACK
                }
            }else if(status == CD){
                cd(protocol.getDataAsString());
            }else if(status == LS){
                string output = ls(protocol.getDataAsString());
                //TODO: send output back
            }else if(status == PUT){
                //TODO
            }else if(status == GET){
                //TODO
                //intesristing link: http://stackoverflow.com/questions/15138353/reading-the-binary-file-into-the-vector-of-unsigned-chars
            }
            //TODO: Check window sequence and blablabla
        }
    }
    return 0;
}