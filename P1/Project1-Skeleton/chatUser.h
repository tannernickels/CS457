#ifndef CHAT_USER_H
#define CHAT_USER_H

#include <string>
using std::string;
using std::to_string;
#include "tcpUserSocket.h"
using std::shared_ptr;
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
#include "cmds.h"

class chatUser{

public: 

    string nickName; 
    void onEvent(Command cmd, string& msg);
    void setSocket(shared_ptr<cs457::tcpUserSocket> clientSocket);

private:

    string username;
    string password; 
    shared_ptr<cs457::tcpUserSocket> connection; 
    
};

#endif