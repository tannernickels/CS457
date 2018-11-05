#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include <map>
using std::map;
using std::make_pair;
#include <string>
using std::string;
using std::to_string;
using std::tolower;
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
#include "CommandLookup.h"
#include "cmds.h"
#include "chatUser.h"
#include "server.h"


typedef map<Command, string> ProcessorMap;
typedef map<string, Handler> HandlerMap;

// This class establishes the chain of responsibility for tasks sent to the server.
// A command is directed to either the server or chatUser onEvent().  
class eventHandler{

    public:
        eventHandler(){initializeMap();}
        
        inline Handler getProcessor(Command cmd){ return handler_map[processor_map[cmd]]; }
        void processTask(string msg, chatUser user, server s);
        void processCommand(string msg, chatUser user, server s);
        void processMessage(string msg, chatUser user);
        inline bool isCommand(string msg){ return msg.at(0) == '/'; }

    private:

        CommandLookup cl;
        ProcessorMap processor_map;
        HandlerMap handler_map;
        void initializeMap();
        
};


#endif