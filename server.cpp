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
        // cout << "status: " << status << endl;
        // cout << protocol.getDataAsString() << endl;
        try{
            if(status == NOISE){
                continue;
            }
            if(status == CD){
                cout << "Recebeu CD\n";
                // cout << "CD: " << receiveProtocol.getDataAsString() << endl;
                cd(receiveProtocol.getDataAsString());
                sendProtocol.setData(vector<BYTE>(1,(BYTE)0), OK);
                sendProtocol.sendMessage(sockt,0);
            }else if(status == LS){
                cout << "Recebeu LS\n";
                // cout << "protocol data: " << receiveProtocol.getDataAsString() << endl;
                // cout << "message data: " << receiveProtocol.getMessages().back().getDataAsString() << endl;
                string output = ls(receiveProtocol.getMessages().back().getDataAsString());
                // cout << "LS: " << output << endl;
                sendProtocol.setData(vector<BYTE>(output.begin(), output.end()), OUTPUT);
                sendProtocol.transmit(sockt, WAIT_STOP);
                // cout << "finished transmit" << endl;
            }else if(status == PUT){
                cout << "Recebeu PUT\n";
                string fileName = receiveProtocol.getDataAsString();
                // cout << "fileName: " << fileName <<endl;
                sendProtocol.setData(vector<BYTE>(1,(BYTE)0), OK);
                sendProtocol.sendMessage(sockt,0);
                receiveProtocol.reset();
                receiveProtocol.receive(sockt,SIZE,WAIT_STOP,false);
                // cout << "fileSize: " << receiveProtocol.getDataAsString() <<endl;
                unsigned long long fileSize = stoi(receiveProtocol.getDataAsString());
                sendProtocol.reset();
                if(hasEnoughSpace(fileSize)){
                    sendProtocol.setData(vector<BYTE>(1,(BYTE)0), OK);
                    sendProtocol.sendMessage(sockt,0);
                }else{
                    sendProtocol.setData(vector<BYTE>(1,(BYTE)SPACE_ERR), ERROR);
                    sendProtocol.sendMessage(sockt,0);
                    continue;
                }
                receiveProtocol.reset();
                receiveProtocol.receive(sockt,DATA,SLIDING,true);
                // cout <<"conteudo: "<< receiveProtocol.getDataAsString()<<endl;
                writeFile(getWorkingPath()+"/"+fileName,receiveProtocol.getData());

            }else if(status == GET){
                cout << "Recebeu GET\n";
                string filePath = getWorkingPath()+"/"+receiveProtocol.getDataAsString();
                if(fexists(filePath)) {
                    string size = to_string(filesize(filePath));
                    // cout << "ARQUIVO: " << filePath << "|" << size << endl;
                    sendProtocol.setData(vector<BYTE>(size.begin(), size.end()), SIZE);
                    sendProtocol.sendMessage(sockt, 0);
                    int error = receiveProtocol.receive(sockt, OK, WAIT_STOP, false);
                    if(error < 0) continue;
                    sendProtocol.reset();
                    ifstream putFile (filePath);
                    stringstream buffer;
                    buffer << putFile.rdbuf();
                    string data = buffer.str();
                    sendProtocol.setData(vector<BYTE>(data.begin(), data.end()), DATA);
                    sendProtocol.transmit(sockt, SLIDING);
                } else {
                    sendProtocol.setData(vector<BYTE>(1,(BYTE)DIR_ERR), ERROR);
                    sendProtocol.sendMessage(sockt,0);
                    continue;
                }
            }else if(status == ENDTX){
                sendProtocol.reset();
                vector<BYTE> val(1,(BYTE)receiveProtocol.getMessages().back().sequence.to_ulong());
                sendProtocol.setData(val, ACK);
                sendProtocol.sendMessages(sockt);
            }else if(status < 0 && status != NOISE){
                sendProtocol.setData(vector<BYTE>(1,(BYTE)NACK), ERROR);
                sendProtocol.sendMessage(sockt,0);
            }
        }catch(char const* strException){
            cout << "Erro:" <<strException <<endl;
        }
        sendProtocol.reset();
        receiveProtocol.reset();
    }
    return 0;
}
