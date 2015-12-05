#include <stdexcept>
#include "definitions.h"
#include "dirFunctions.h"
#include "ConexaoRawSocket.c"
#include "Protocol.h"

void printCommandsList();

int main(){
    int pos, sockt = ConexaoRawSocket(DEVICE);
    string line, command, args;
    Protocol protocol = Protocol();
    printCommandsList();
    while(true){
        cout << endl << "Entre com o comando:" << endl;
        getline(cin,line);
        pos = line.find_first_of(" ");
        if(pos == string::npos){
            pos = line.size();
        }
        command = line.substr(0,pos);
        try{
            if(command == "quit"){
                break;
            }
            if(command == "cd"){
                args = line.substr(pos+1, line.size());
                cd(args);
            }else if(command == "ls"){
                cout << ls(line);
            }else if(command == "cdr"){
                args = line.substr(pos+1, line.size());
                protocol.setData(vector<BYTE>(args.begin(), args.end()), CD);
                protocol.sendMessages(sockt, WAIT_STOP);
            }else if(command == "lsr"){
                Message msg = Message();
                line.replace(line.find("lsr"), string("lsr").length(), "ls");
                protocol.setData(vector<BYTE>(line.begin(), line.end()), LS);
                protocol.sendMessages(sockt, WAIT_STOP);
                // TODO: imprimir resposta
            }else if(command == "put"){
                //TODO
            }else if(command == "get"){

            }else if(command == "help"){
                printCommandsList();
            }else{
                cout << "Comando inexistente." << endl;
                printCommandsList();
            }
        }catch(char const* strException){
            cerr<<"Error: "<< strException << endl;
        }catch(out_of_range e){
            cerr<<"Error: Esse comando requer argumentos."<<endl;
        }

        // int status = protocol.recvMessage(sockt);
        // cout << "Status: " << status << endl;
        // if(status > 0){
        //     if(status == ENDTX){
        //         protocol = Protocol();
        //         //TODO: send ACK
        //     }else if(status == CD){
        //         cd(protocol.getDataAsString());
        //     }else if(status == LS){
        //         string output = ls(protocol.getDataAsString());
        //         //TODO: send output back
        //     }else if(status == PUT){
        //         //TODO
        //     }else if(status == GET){
        //         //TODO
        //     }
        // }
    }
    return 0;
}

void printCommandsList(){
    cout << "Os comandos suportados são:"<< endl;
    cout << "cd - Mudar de diretório local"<< endl;
    cout << "ls - Visualizar arquivos no diretório local"<< endl;
    cout << "cdr - Mudar de diretório remoto"<< endl;
    cout << "lsr - Visualizar arquivos no diretório remoto"<< endl;
    cout << "put - Enviar arquivo para diretório remoto"<< endl;
    cout << "get - Pegar arquivo do diretório remoto"<< endl;
    cout << "help - Lista de comandos"<< endl;
    cout << "quit - Sair"<< endl;
}
