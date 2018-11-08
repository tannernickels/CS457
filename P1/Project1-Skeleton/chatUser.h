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
    
    // PUBLIC USER FIELDS
    string nickName; 
    bool isAway;
    string away_msg;

    // TASK PROCESSOR
    void onEvent(Command cmd, vector<string>& args);
    inline void writeToSocket(string message){this -> connection.get()->sendString(message);};

    // ACCESSORS
    inline shared_ptr<cs457::tcpUserSocket> socketPointer(){ return this -> connection;}
    inline string& getUsername(){return this -> username;}
    inline string& getIP(){ return this -> ip; }

    // MUTATORS
    void setSocket(shared_ptr<cs457::tcpUserSocket> clientSocket);
    void setLevel(string& level);
    inline void joinChatRoom(string& channel_name) { rooms.push_back(channel_name); }
    inline void setIP(string& ip) { this->ip = ip; }
    
    //Events
    void away(vector<string>& args);
    void help();
    void info();
    void nick(vector<string>& args);
    void set_name(vector<string>& args);
    void ping(vector<string>& args);
    void invalid(vector<string>& args);

private:

    string username;
    Level level;
    vector<string> user_chatrooms;
    shared_ptr<cs457::tcpUserSocket> connection; 
    int id;
    int usage(int code);
    vector<string> rooms;
    string ip;
};

#endif