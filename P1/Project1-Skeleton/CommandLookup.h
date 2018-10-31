#ifndef COMMAND_LOOKUP_H
#define COMMAND_LOOKUP_H

#include <map>
using std::map;
using std::make_pair;
#include <string>
using std::string;
using std::to_string;
#include <sstream>
using std::istringstream;
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
#include <vector>
using std::vector;
#include "cmds.h"

<<<<<<< HEAD
enum Command {AWAY, CONNECT, DIE, HELP,
                INFO, INVITE, ISON, JOIN,
                KICK, KILL, KNOCK, LIST,
                MODE, NICK, NOTICE, OPER,
                PART, PASS, PING, PONG,
                PRIVMSG, QUIT, RESTART, RULES,
                SETNAME, SILENCE, TIME, TOPIC,
                USER, USERHOST, USERIP, USERS,
                VERSION, WALLOPS, WHO, WHOIS};
=======
>>>>>>> 576279f193904b95319ec6bc757bb8f4d18f9f91
typedef map<string, Command> CommandMap;

class CommandLookup{

public:

    CommandLookup(){initializeMap();}

    void initializeMap();
    vector<string> parseArguments(string& args);
    int execute(string& command);
    int usage(int code);
    int find(string& command);
    void printVector(vector<string>& v);

private:
    
    CommandMap command_map;

    void quit(vector<string>& args);
    void info();

};

#endif