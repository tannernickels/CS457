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

    chatUser(string& user, shared_ptr<cs457::tcpUserSocket> clientSocket) : username(user), connection(clientSocket){}
    chatUser(){}
    string nickName; 
    void onEvent(Command cmd, string& msg);
    void setSocket(shared_ptr<cs457::tcpUserSocket> clientSocket);

    inline const string& getUsername(){return this -> username;}




private:

    string username;
    shared_ptr<cs457::tcpUserSocket> connection; 
    
};

#endif