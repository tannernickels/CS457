#ifndef COMMAND_LOOKUP_H
#define COMMAND_LOOKUP_H

#include <algorithm>
using std::transform;
#include <map>
using std::map;
using std::make_pair;
#include <string>
using std::string;
using std::to_string;
using std::tolower;
#include <sstream>
using std::istringstream;
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
#include <vector>
using std::vector;
#include "cmds.h"

typedef map<string, Command> CommandMap;

class CommandLookup{

public:

    CommandLookup(){initializeMap();}

    void initializeMap();
    vector<string> parseArguments(string& args);
    int execute(string& command);
    int usage(int code);
    Command find(string& command);
    Command getCommand(vector<string>& args);
    void printVector(vector<string>& v);
    void to_lowercase(string& s);

private:
    
    CommandMap command_map;

    void quit(vector<string>& args);

};

#endif