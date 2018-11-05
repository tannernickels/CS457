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
#include <vector>
using std::vector;

class chatUser{

public: 

    chatUser(string& user, shared_ptr<cs457::tcpUserSocket> clientSocket) : username(user), connection(clientSocket){}
    chatUser(){}
    
    string nickName; 
    bool isAway;
    string away_msg;

    void onEvent(Command cmd, vector<string>& args);
    void setSocket(shared_ptr<cs457::tcpUserSocket> clientSocket);
    inline shared_ptr<cs457::tcpUserSocket> socketPointer(){ return this -> connection;}

    inline string& getUsername(){return this -> username;}
    int usage(int code);



private:

    string username;
    shared_ptr<cs457::tcpUserSocket> connection; 
    
};

#endif