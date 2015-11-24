#include "definitions.h"
#include "dirFunctions.h"

int main(){
    
    while(true){
        string command, path;
        cout << endl << "Entre com o comando:" << endl;
        cin >> command;
        if(command == "quit"){
            break;
        }
        cin >> path;
        if(command == "cd"){
            cd(path);
        }else if(command == "ls"){
            ls(path);
        }else if(command == "cdr"){
            //TODO
        }else if(command == "lsr"){
            //TODO
        }else if(command == "put"){
            //TODO
        }else if(command == "get"){
            //TODO
        }
    }
    return 0;
}