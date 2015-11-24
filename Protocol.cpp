#include "Protocol.h"
#include "definitions.h"

Message Protocol::getMessage(){
    return message;
}

void Protocol::setMessage(Message message){
    this->message = message;
}

Protocol::Protocol(){
    message.begin = 0x7E;
}