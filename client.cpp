#include <sstream>
#include "definitions.h"
#include "dirFunctions.h"
#include "ConexaoRawSocket.c"

void printCommandsList();
vector<string> getArgs();

int main(){
    int sockt = ConexaoRawSocket(DEVICE);
    vector<string> args;
    printCommandsList();
    while(true){
        cout << endl << "Entre com o comando:" << endl;
        args = getArgs();
        if(args[0] == "quit"){
            break;
        }
        if(args[0] == "cd"){
            cd(args[1]);
        }else if(args[0] == "ls"){
            try{
                cout << ls(args);
            }catch(char const* strException){
                cerr<<"Error: "<< strException << endl;
            }
        }else if(args[0] == "cdr"){
            //TODO
        }else if(args[0] == "lsr"){
            //TODO
        }else if(args[0] == "put"){
            //TODO
        }else if(args[0] == "get"){

        }else if(args[0] == "help"){
            printCommandsList();
        }else{
            cout << "Comando inexistente." << endl;
            printCommandsList();

        }
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

vector<string> getArgs(){
    string line, arg;
    getline(cin,line);
    stringstream ss(line);
    vector<string> args;
    while(getline(ss, arg, ' ')){
        if(!arg.empty()){
            args.push_back(arg);
        }
    }
    return args;
}