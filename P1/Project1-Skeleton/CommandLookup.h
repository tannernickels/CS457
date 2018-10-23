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

enum Command {AWAY, CONNECT, DIE, HELP,
                INFO, INVITE, ISON, JOIN,
                KICK, KILL, KNOCK, LIST,
                MODE, NICK, NOTICE, OPER,
                PART, PASS, PING, PONG,
                PRIVMSG, QUIT=2, RESTART, RULES,
                SETNAME, SILENCE, TIME, TOPIC,
                USER, USERHOST, USERIP, USERS,
                VERSION, WALLOPS, WHO, WHOIS};
typedef map<string, Command> CommandMap;

class CommandLookup{

public:

    CommandLookup(){initializeMap();}

    void initializeMap();
    vector<string> parseArguments(string& args);
    int execute(string& command);
    int usage(int code);

    void printVector(vector<string>& v);

private:
    
    CommandMap command_map;

    void quit(vector<string>& args);


};

#endif