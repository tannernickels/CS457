#include "CommandLookup.h"

void CommandLookup::initializeMap(){
    command_map.insert(make_pair("/away", AWAY));
    command_map.insert(make_pair("/connect", CONNECT));
    command_map.insert(make_pair("/die", DIE));
    command_map.insert(make_pair("/help", HELP));
    command_map.insert(make_pair("/info", INFO));
    command_map.insert(make_pair("/invite", INVITE));
    command_map.insert(make_pair("/ison", ISON));
    command_map.insert(make_pair("/join", JOIN));
    command_map.insert(make_pair("/kick", KICK));
    command_map.insert(make_pair("/kill", KILL));
    command_map.insert(make_pair("/knock", KNOCK));
    command_map.insert(make_pair("/list", LIST));
    command_map.insert(make_pair("/mode", MODE));
    command_map.insert(make_pair("/nick", NICK));
    command_map.insert(make_pair("/notice", NOTICE));
    command_map.insert(make_pair("/oper", OPER));
    command_map.insert(make_pair("/part", PART));
    command_map.insert(make_pair("/pass", PASS));
    command_map.insert(make_pair("/ping", PING));
    command_map.insert(make_pair("/pong", PONG));
    command_map.insert(make_pair("/privmsg", PRIVMSG));
    command_map.insert(make_pair("/quit", QUIT));
    command_map.insert(make_pair("/restart", RESTART));
    command_map.insert(make_pair("/rules", RULES));
    command_map.insert(make_pair("/setname", SETNAME));
    command_map.insert(make_pair("/silence", SILENCE));
    command_map.insert(make_pair("/time", TIME));
    command_map.insert(make_pair("/topic", TOPIC));
    command_map.insert(make_pair("/user", USER));
    command_map.insert(make_pair("/userhost", USERHOST));
    command_map.insert(make_pair("/userip", USERIP));
    command_map.insert(make_pair("/users", USERS));
    command_map.insert(make_pair("/version", VERSION));
    command_map.insert(make_pair("/wallops", WALLOPS));
    command_map.insert(make_pair("/who", WHO));
    command_map.insert(make_pair("/whois", WHOIS));
}

int CommandLookup::execute(string& command){
    
    Command c = command_map[command.substr(0, command.length()-2)];
    cout << command_map.size() << endl;
    switch(c){
        case QUIT:
            quit();
            break;
        default:
            cout << "NOT FOUND" << endl;

    }
}

int CommandLookup::usage(){
    return -1;
}

void CommandLookup::quit(){
    exit(-1);
}