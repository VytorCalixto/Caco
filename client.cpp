#include <stdexcept>
#include "definitions.h"
#include "dirFunctions.h"
#include "ConexaoRawSocket.c"
#include "Protocol.h"

void printCommandsList();

int main(){
    int pos, sockt = ConexaoRawSocket(DEVICE);
    string line, command, args;
    Protocol receiveProtocol, sendProtocol;
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
                sendProtocol.setData(vector<BYTE>(args.begin(), args.end()), CD);
                sendProtocol.sendMessage(sockt, 0);
                receiveProtocol.receive(sockt, OK, WAIT_STOP, false);
                cout << "Diretório remoto: " << args;
            }else if(command == "lsr"){
                line.replace(line.find("lsr"), string("lsr").length(), "ls");
                sendProtocol.setData(vector<BYTE>(line.begin(), line.end()), LS);
                sendProtocol.sendMessage(sockt,0);
                cout << "Remoto:" << endl;
                receiveProtocol.receive(sockt, OUTPUT, WAIT_STOP, true);
                cout << receiveProtocol.getDataAsString() << endl;
            }else if(command == "put"){
                args = line.substr(pos+1, line.size());
                if(fexists(args)) {
                    string size = to_string(filesize(args));
                    cout << "ARQUIVO: " << args << "|" << size << endl;
                    sendProtocol.setData(vector<BYTE>(args.begin(), args.end()), PUT);
                    sendProtocol.sendMessage(sockt, 0);
                    int error = receiveProtocol.receive(sockt, OK, WAIT_STOP, false);
                    if(error < 0) continue;
                    sendProtocol.reset();
                    sendProtocol.setData(vector<BYTE>(size.begin(), size.end()), SIZE);
                    sendProtocol.sendMessage(sockt, 0);
                    error = receiveProtocol.receive(sockt, OK, WAIT_STOP, false);
                    if(error < 0) continue;
                    sendProtocol.reset();
                    ifstream putFile (args);
                    stringstream buffer;
                    buffer << putFile.rdbuf();
                    string data = buffer.str();
                    sendProtocol.setData(vector<BYTE>(data.begin(), data.end()), DATA);
                    sendProtocol.transmit(sockt, SLIDING);
                } else {
                    cout << "ERROR: arquivo não existe\n";
                }
                sendProtocol.reset();
                receiveProtocol.reset();
            }else if(command == "get"){
                args = line.substr(pos+1, line.size());
                sendProtocol.setData(vector<BYTE>(args.begin(), args.end()), GET);
                sendProtocol.sendMessage(sockt, 0);
                int error = receiveProtocol.receive(sockt, SIZE, WAIT_STOP, false);
                if(error < 0) continue;
                string s_size = receiveProtocol.getDataAsString();
                cout << s_size << endl;
                unsigned int fileSize = stoi(s_size);
                cout << "Tamanho: " << fileSize << endl;
                sendProtocol.reset();
                if(hasEnoughSpace(fileSize)) {
                    sendProtocol.setData(vector<BYTE>(1,(BYTE)0), OK);
                    sendProtocol.sendMessage(sockt,0);
                } else {
                    cout << "ERROR: não há espaço disponível\n";
                    sendProtocol.setData(vector<BYTE>(1,(BYTE)SPACE_ERR), ERROR);
                    sendProtocol.sendMessage(sockt,0);
                    continue;
                }
                receiveProtocol.reset();
                receiveProtocol.receive(sockt,DATA,SLIDING,true);
                cout <<"conteudo: "<< receiveProtocol.getDataAsString()<<endl;
                writeFile(getWorkingPath()+"/"+args,receiveProtocol.getData());
                sendProtocol.reset();
                receiveProtocol.reset();
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
        sendProtocol.reset();
        receiveProtocol.reset();
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
