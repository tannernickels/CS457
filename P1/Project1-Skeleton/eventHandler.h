#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include <map>
using std::map;
using std::make_pair;
#include <string>
using std::string;
using std::to_string;
using std::tolower;
#include "cmds.h"
#include "chatUser.h"
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
#include "CommandLookup.h"


typedef map<Command, string> ProcessorMap;
typedef map<string, Handler> HandlerMap;
// This class will be used in driver to establish chain of responsibility for commands, directs data to either the server or chatUser.  

class eventHandler{

    public:

        eventHandler(){initializeMap();}
        
        // Returns "server" or "chatUser"
        inline Handler getProcessor(Command cmd){ return handler_map[processor_map[cmd]]; }
        void processTask(string msg, chatUser user);
        void processCommand(string msg, chatUser user);
        void processMessage(string msg, chatUser user);
        inline bool isCommand(string msg){ return msg.at(0) == '/'; }

    private:

        CommandLookup cl;
        ProcessorMap processor_map;
        HandlerMap handler_map;
        void initializeMap();
        
};


#endif