#ifndef CHAT_ROOM_H
#define CHAT_ROOM_H

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

    inline void addUser(chatUser user){ users.push_back(user); }

    // GETTERS
    inline string getChannelName(){ return this->channel_name; }
    inline string getChannelDecription(){ return this->description; }
    
    // SETTERS
    inline changeName(string name) { this->channel_name = name; }
    inline changeDescription(string d) { this->description = d; }
    inline changePassword(string pwd) { this->password = pwd; }

private:
    string channel_name;
    string description;
    string password;
    vector<chatUser> users;


};

#endif