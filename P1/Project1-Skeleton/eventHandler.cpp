#include "eventHandler.h"

// HIGHEST-LEVEL OF REQUEST PROCESSING
void eventHandler::processTask(string msg, chatUser& user, server& server){
 bool type = isCommand(msg);
 switch(type){
     case COMMAND:processCommand(msg, user, server); break;
     case MESSAGE:processMessage(msg, user); break;

 }
}


// MESSAGE PROCESSING
void eventHandler::processMessage(string msg, chatUser& user){
    cout << "message was not a command.." << endl;
    user.writeToSocket(msg); // relay message back to client to confirm acknowledgement
}

// COMMAND PROCESSING
void eventHandler::processCommand(string msg, chatUser& user, server& server){
    
    vector<string> args = cl.parseArguments(msg);
    Command command = cl.getCommand(args);
    Handler handler = getProcessor(command);
    
    switch(handler){
        case SERVER: server.onEvent(command, args, user); break;
        case CHATUSER: user.onEvent(command, args); break; 
    }
}

// THIS DECIDES WHERE COMMANDS ARE PROCESSED
// TODO: I made quick judgements on where I thought commands should go so this is subject to change
void eventHandler::initializeMap(){
    processor_map.insert(make_pair(AWAY, "chatUser"));
    processor_map.insert(make_pair(CONNECT, "server"));
    processor_map.insert(make_pair(DIE, "server"));
    processor_map.insert(make_pair(HELP, "chatUser"));
    processor_map.insert(make_pair(INFO, "chatUser"));
    processor_map.insert(make_pair(INVALID, "chatUser"));
    processor_map.insert(make_pair(INVITE, "server"));
    processor_map.insert(make_pair(ISON, "server"));
    processor_map.insert(make_pair(JOIN, "server"));
    processor_map.insert(make_pair(KICK, "server"));
    processor_map.insert(make_pair(KILL, "server"));
    processor_map.insert(make_pair(KNOCK, "server"));
    processor_map.insert(make_pair(LIST, "server"));
    processor_map.insert(make_pair(MODE, "server"));
    processor_map.insert(make_pair(MSG, "server"));
    processor_map.insert(make_pair(NICK, "chatUser"));
    processor_map.insert(make_pair(NOTICE, "server"));
    processor_map.insert(make_pair(OPER, "server"));
    processor_map.insert(make_pair(PART, "server"));
    processor_map.insert(make_pair(PASS, "server"));
    processor_map.insert(make_pair(PING, "chatUser"));
    processor_map.insert(make_pair(PONG, "chatUser"));
    processor_map.insert(make_pair(PRIVMSG, "server"));
    processor_map.insert(make_pair(QUIT, "server"));
    processor_map.insert(make_pair(RESTART, "server"));
    processor_map.insert(make_pair(RULES, "server"));
    processor_map.insert(make_pair(SETNAME, "chatUser"));
    processor_map.insert(make_pair(SILENCE, "server"));
    processor_map.insert(make_pair(TIME, "server"));
    processor_map.insert(make_pair(TOPIC, "server"));
    processor_map.insert(make_pair(USER, "chatUser"));
    processor_map.insert(make_pair(USERHOST, "server"));
    processor_map.insert(make_pair(USERIP, "server"));
    processor_map.insert(make_pair(USERS, "server"));
    processor_map.insert(make_pair(VERSION, "server"));
    processor_map.insert(make_pair(WALLOPS, "server"));
    processor_map.insert(make_pair(WHO, "server"));
    processor_map.insert(make_pair(WHOIS, "server"));


    handler_map.insert(make_pair("server", SERVER));
    handler_map.insert(make_pair("chatUser", CHATUSER));
}