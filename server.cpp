#include "definitions.h"
#include "ConexaoRawSocket.c"

int main(){
	int sockt = ConexaoRawSocket(DEVICE);
	Protocol protocol;
	while(true){
		int ret = protocol.readMessage(sockt);
		if(ret){
			if(ret == 1){
				//End of transmission
			}
			//TODO: Check window sequence and blablabla
		}
	}
	return 0;
}