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
    command_map.insert(make_pair("/msg", MSG));
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

vector<string> CommandLookup::parseArguments(string& args){
    istringstream iss(args);
    vector<string> arguments;
    do{
        string buf;
        iss >> buf;
        if(buf.size()>0){
            arguments.push_back(buf);
        }
    }while(iss);

    return arguments;

}

int CommandLookup::execute(string& command){
    
    //Grab individual arguments from the msg string and store in a vector
    vector<string> args = parseArguments(command);

    //Grab the command enum using the first argument, then remove it from the vector
    to_lowercase(args[0]);
    Command c = command_map[args[0]];
    args.erase(args.begin());

    switch(c){
        case QUIT:
            quit(args);
            break;
        default:
            cout << "NOT FOUND" << endl;

    }
}

int CommandLookup::usage(int code){
    string u = "USAGE(" + to_string(code) + "): ";
    switch(code){
        case -1:
            cout << u << "incorrect number of arguments" << endl;
            return code;
        case -2:
            cout << u << "incorrect argument error" << endl;
        default:
            return code;
    }
}

void CommandLookup::printVector(vector<string>& v){
    for(auto& arg : v){ 
        cout << "|" << arg << "|" << endl;
    }

}

void CommandLookup::quit(vector<string>& args){
    if(args.size()==0){
        exit(-1);
    }else{
        string message = "";
        for (auto& word : args){
            if(args[0][0]!='\"'){
                cout << "QUIT: message must be encased in quotations:\ne.x. \"This is a message\"" << endl;
                usage(-2);
                return;
            }
            message += word + " ";
        }
        cout << message << endl;
        exit(-1);
    }
}

// returns enum of command
Command CommandLookup::find(string& command){
    //Grab individual arguments from the msg string and store in a vector
    vector<string> args = parseArguments(command);
    //Grab the command enum using the first argument, then remove it from the vector
    to_lowercase(args[0]);
    Command cmd = command_map[args[0]];
    if(cmd == 0 && args[0] != "/away")
        return INVALID; // return INVALID if the command was invalid

    args.erase(args.begin());
    return cmd;
}

Command CommandLookup::getCommand(vector<string>& args){
    //to_lowercase(args[0]);
    Command c = command_map[args[0]];
    if(c == 0 && args[0] != "/away")
        c = INVALID;
    args.erase(args.begin());
    return c;
}

void CommandLookup::to_lowercase(string& s){
    for (int i = 0; i < s.size(); i++){
        s[i] = tolower(s[i]);
    }
    return;

}