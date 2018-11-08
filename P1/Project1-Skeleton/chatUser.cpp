#include "chatUser.h"

void chatUser::setSocket(shared_ptr<cs457::tcpUserSocket> clientSocket){
    connection = clientSocket;
}

void chatUser::setLevel(string& level){
    Level l;
    if(level=="user"){
        l = SLUGS;
    }else if(level=="channelop"){
        l = CHANNELOP;
    }else if(level=="sysop"){
        l = SYSOP;
    }else if(level=="admin"){
        l = ADMIN;
    }else{
        return;
    }
}

void chatUser::onEvent(Command cmd, vector<string>& args){
 
     switch(cmd){
        case AWAY:  away(args);
                    break;
        case HELP:  help(); 
                    break;
        case INFO:  info();
                    break;
        case NICK:  nick(args);
                    break;
        case PING:  ping(args);
                    break;
        case PONG: std::cout << "execute PONG()" << std::endl; break;
        case SETNAME:   set_name(args);
                        break;
        case USER: std::cout << "execute USER()" << std::endl; break;
        case INVALID:   invalid(args);
                        break;
        default: std::cout << "ERROR IN COMMAND PROCESSING" << std::endl; break;
    }

}

void chatUser::invalid(vector<string>& args){
    std::cout << "invalid command" << endl;
    connection.get()->sendString("The command, " + args[0] + ", is invalid. Type /Help for list of commands.");
}
void chatUser::away(vector<string>& args){
    cout << "executing AWAY()" << endl; 
    if(args.size()==0){
        isAway = false;
        connection.get()->sendString("Away mode set to off");
    }else{
        isAway = true;
        for (unsigned int i = 0; i < args.size(); i++){
            if(i==args.size()-1){
                away_msg += args[i];
            }else{
                away_msg += args[i] + ' ';
            }
        }
        connection.get()->sendString("Away mode set to on. Message displayed while away:\n" + away_msg);
    }
}

void chatUser::help(){
    FileIO reader("db/help.txt", "r");
    string help_msg = reader.readFull();
    connection.get()->sendString(help_msg);
}

void chatUser::info(){
    cout << "executing INFO()" << endl; 
    connection.get()->sendString("This IRC server is made possible by YUNG T and BIG DEV");
}

void chatUser::nick(vector<string>& args){
    cout << "executing NICK()" << endl; 
    if(args.size()==1){
        nickName = args[0];
        cout << "[Client "<< id << "] Registered the nickname: " << nickName << endl;
        connection.get() -> sendString("You registerd the nickname: " + this -> nickName);
    } else{
        usage(-1);
    }
}

void chatUser::set_name(vector<string>& args){
    std::cout << "execute SETNAME()" << std::endl;
    // Allows a client to change the "real name" -- is the username the same as the "real name"?
    this -> username = args[0];
    connection.get() -> sendString("You registerd the username: " + this -> username);
}


// TODO: if an arg is provided then message the user with the provided nickname "/PONG [name of user who first issued commmand]",
//       the reciver of that command can then writeback with "/PONG",
//       so if you recieve that command with no args you know to calculate the latency between yourself and that user
// currently it just sends a /PONG command to the user, where the latency is calculated. 
void chatUser::ping(vector<string>& args){
    std::cout << "execute PING()" << std::endl;
    connection.get() -> sendString("/PONG");
}

int chatUser::usage(int code){
    string u = "USAGE(" + to_string(code) + "): ";
    switch(code){
        case -1:
            cout << u << "incorrect number of arguments" << endl;
            connection.get() -> sendString(u + "incorrect number of arguments");
            return code;
        case -2:
            cout << u << "incorrect argument error" << endl;
            connection.get() -> sendString(u + "incorrect argument error");
            return code;
        default:
            return code;
    }
}


