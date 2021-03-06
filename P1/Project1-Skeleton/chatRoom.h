#ifndef CHAT_ROOM_H
#define CHAT_ROOM_H

#include <map>
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
#include "chatUser.h"

class chatRoom{

public:
    chatRoom(string name, string d, string pwd): channel_name(name), description(d), password(pwd) {}
    chatRoom(){}

    inline void addUser(chatUser& user){ users.push_back(user); }
    bool isValidUser(chatUser& user);
    void removeUser(chatUser& user);

    // GETTERS
    inline string getChannelName(){ return this->channel_name; }
    inline string getChannelDescription(){ return this->description; }
    inline string getChannelPassword(){ return this->password; }
    
    // SETTERS
    inline void changeName(string& name) { this->channel_name = name; }
    inline void changeDescription(string& d) { this->description = d; }
    inline void changePassword(string& pwd) { this->password = pwd; }

    //CHAT METHODS
    void joinChannel(chatUser& user, string& password);
    void sendMessageToChannel(string& message, chatUser& sender, bool isKnock);
    void printChannelData();


private:
    string channel_name;
    string description;
    string password;
    vector<chatUser> users;


};

#endif