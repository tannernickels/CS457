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
#include "FileIO.h"

class chatUser{

public: 

    // CONSTRUCTORS
    chatUser(string& user, shared_ptr<cs457::tcpUserSocket> clientSocket, int id_num) : username(user), connection(clientSocket), id(id_num){}
    chatUser(){}
    
    // USER FIELDS
    string nickName; 
    bool isAway;
    string away_msg;

    // TASK PROCESSOR
    void onEvent(Command cmd, vector<string>& args);
    

    // ACCESSORS
    inline shared_ptr<cs457::tcpUserSocket> socketPointer(){ return this -> connection;}
    inline string& getUsername(){return this -> username;}

    // MUTATORS
    void setSocket(shared_ptr<cs457::tcpUserSocket> clientSocket);
    
    //Events
    void away(vector<string>& args);
    void help();
    void info();
    void nick(vector<string>& args);
    //set username?? 
    void set_name(vector<string>& args);

private:

    string username;
    shared_ptr<cs457::tcpUserSocket> connection; 
    int id;
    int usage(int code);

};

#endif